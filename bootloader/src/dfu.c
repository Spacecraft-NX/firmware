/*
 * Copyright (c) 2020 Spacecraft-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <dfu.h>
#include <gd32f3x0.h>
#include <string.h>
#include <leds.h>

void jump_to_app(uint32_t addr, struct bootloader_usb *usb);

enum DFU_ERRORS
{
	ERROR_SUCCESS = 0x70000000,
	ERROR_INVALID_PACKAGE_LENGTH = 0x40000000,
	ERROR_INVALID_OFFSET,
	ERROR_INVALID_LENGTH,
	ERROR_ERASE_FAILED,
	ERROR_FLASH_FAILED,
	ERROR_FAILED_TO_UPDATE_OB,

	ERROR_UNIMPLEMENTED = 0x50000000
};

enum COMMANDS
{
	PING = 0xA0F0,
	SET_OFFSET,
	READ_FLASH,
	READ_OB,
	SET_OB,
};

char erase_flash(uint8_t *dest)
{
	fmc_unlock();
	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
	if(fmc_page_erase((uint32_t) dest))
	{
		fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
		fmc_lock();
		return 0;
	}

	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
	fmc_lock();
	return 1;
}

char burn_flash(uint8_t *dest, uint8_t *src, uint32_t len)
{
	fmc_unlock();
	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

	for (unsigned int i = 0; i < len; i += 4)
	{
		if (fmc_word_program((uint32_t) dest + i, *(uint32_t *)(src + i)))
		{
			fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
			fmc_lock();
			return 0;
		}
	}

	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
	fmc_lock();
	return 1;
}

char update_ob_protection(uint8_t ob_spc)
{
	fmc_unlock();
	ob_unlock();
	char ret = !(ob_erase() || ob_security_protection_config(ob_spc));
	ob_lock();
	fmc_lock();
	return ret;
}

void send32(struct bootloader_usb *usb, uint32_t value)
{
	*(uint32_t *) usb->send_buffer = value;
	usb->send_data(4);
}

uint32_t g_offset = 0;

void dfu(struct bootloader_usb *usb)
{
	while (1)
	{
		leds_set_pattern_delayed(&lp_usb, 1000); // don't overwrite old status for 1s
		int received_len = 0;
		do
		{
			usb->wait_till_ready();
			received_len = usb->receive_data();
		}
		while (!received_len);

		if (received_len == 64)
		{
			leds_set_pattern(&lp_fw_write);
			if (g_offset < BOOTLOOADER_SIZE || g_offset > (FLASH_SIZE - 64))
			{
				send32(usb, ERROR_INVALID_OFFSET);
				continue;
			}

			if (!(g_offset & 0x3FF))
			{
				if (!erase_flash((uint8_t *) 0x8000000 + g_offset))
				{
					send32(usb, ERROR_ERASE_FAILED);
					continue;
				}
			}

			if (!burn_flash((uint8_t *) 0x8000000 + g_offset, usb->receive_buffer, 64))
			{
				send32(usb, ERROR_FLASH_FAILED);
				continue;
			}

			g_offset += 64;

			send32(usb, ERROR_SUCCESS);
			continue;
		}

		if (received_len == 1)
		{
			leds_off(); // so no IRQs
			jump_to_app(FIRMWARE_START_ADDR, usb);
		}

		switch(*(uint16_t *) usb->receive_buffer)
		{
			case PING:
			{
				if (received_len != 2)
				{
					send32(usb, ERROR_INVALID_PACKAGE_LENGTH);
					break;
				}
				send32(usb, ERROR_SUCCESS);
				break;
			}

			case SET_OFFSET:
			{
				if (received_len != 6)
				{
					send32(usb, ERROR_INVALID_PACKAGE_LENGTH);
					break;
				}

				uint32_t offset = *(uint32_t *)&usb->receive_buffer[2];
				if (offset < BOOTLOOADER_SIZE || offset > (FLASH_SIZE - 64))
				{
					send32(usb, ERROR_INVALID_OFFSET);
					break;
				}

				g_offset = offset;
				send32(usb, ERROR_SUCCESS);
				break;
			}
			case READ_FLASH:
			{
				leds_set_pattern(&lp_fw_read);
				if (received_len != 10)
				{
					send32(usb, ERROR_INVALID_PACKAGE_LENGTH);
					break;
				}

				uint32_t offset = *(uint32_t *)&usb->receive_buffer[2];
				if (offset < BOOTLOOADER_SIZE || offset > (FLASH_SIZE - 64))
				{
					send32(usb, ERROR_INVALID_OFFSET);
					break;
				}

				uint32_t length = *(uint32_t *)&usb->receive_buffer[6];
				if (!length || length > 64)
				{
					send32(usb, ERROR_INVALID_LENGTH);
					break;
				}

				send32(usb, ERROR_SUCCESS);
				memcpy(usb->send_buffer, (uint8_t *) 0x8000000 + offset, length);
				usb->send_data(length);
				break;
			}
			case READ_OB:
			{
				if (received_len != 2)
				{
					send32(usb, ERROR_INVALID_PACKAGE_LENGTH);
					break;
				}

				send32(usb, ERROR_SUCCESS);
				memcpy(usb->send_buffer, (uint8_t *) OB, 12);
				usb->send_data(12);
				break;
			}
			case SET_OB:
			{
				if (received_len != 3)
				{
					send32(usb, ERROR_INVALID_PACKAGE_LENGTH);
					break;
				}

				if (!update_ob_protection(usb->receive_buffer[2]))
					send32(usb, ERROR_FAILED_TO_UPDATE_OB);
				send32(usb, ERROR_SUCCESS);
				break;
			}
		}
	}
}
