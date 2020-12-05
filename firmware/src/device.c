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

#include <gd32f3x0.h>
#include <device.h>
#include <board_id.h>
#include <board.h>
#include <fpga.h>
#include <adc.h>
#include <delay.h>

enum DEVICE_TYPE detect_device_type()
{
	fpga_reset_device(0);
	delay_ms(1);
	enum BOARD_ID bid = board_id_get();
	if (bid == BOARD_ID_LITE)
	{
		adc_init(SWITCH_LITE_ADC_GPIO_PORT, SWITCH_LITE_ADC_GPIO_PIN, 2);
		if (adc_wait_eoc_read() > 0xFF)
			return DEVICE_TYPE_LITE;
	}
	else if (bid == BOARD_ID_CORE)
	{
		adc_init(SWITCH_MARIKO_ADC_GPIO_PORT, SWITCH_MARIKO_ADC_GPIO_PIN, 9);
		int bp1_val = adc_wait_eoc_read();
		gpio_mode_set(SWITCH_MARIKO_ADC_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SWITCH_MARIKO_ADC_GPIO_PIN);

		adc_init(SWITCH_ERISTA_ADC_GPIO_PIN, SWITCH_ERISTA_ADC_GPIO_PORT, 8);
		int bp0_val = adc_wait_eoc_read();
		gpio_mode_set(SWITCH_ERISTA_ADC_GPIO_PIN, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SWITCH_ERISTA_ADC_GPIO_PORT);

		if (bp1_val > 255 && bp0_val <= 256)
				return DEVICE_TYPE_MARIKO;
		if (bp1_val <= 255 && bp0_val > 255)
				return DEVICE_TYPE_ERISTA;
	}
	return DEVICE_TYPE_UNKNOWN;
}