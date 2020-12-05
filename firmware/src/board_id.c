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
#include <board_id.h>
#include <board.h>

void board_id_init()
{
	gpio_mode_set(BOARDID_A_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, BOARDID_A_GPIO_PIN);
	gpio_mode_set(BOARDID_B_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, BOARDID_B_GPIO_PIN);
}

enum BOARD_ID board_id_get()
{
	if (gpio_input_bit_get(BOARDID_B_GPIO_PORT, BOARDID_B_GPIO_PIN) == RESET)
	{
		if (gpio_input_bit_get(BOARDID_A_GPIO_PORT, BOARDID_A_GPIO_PIN) == RESET)
			return BOARD_ID_CORE;
		else
			return BOARD_ID_LITE;
	}

	return BOARD_ID_UNKNOWN;
}