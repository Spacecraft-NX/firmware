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

#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#include <stdint.h>

#define FLASH_SIZE 0x20000
#define BOOTLOOADER_SIZE 0x3000
#define FIRMWARE_START_ADDR (0x8000000 + BOOTLOOADER_SIZE)

struct bootloader_usb
{
	uint8_t *receive_buffer;
	uint8_t *send_buffer;
	int (* receive_data)();
	void (* send_data)(int len);
	void (* wait_till_ready)();
};

#endif
