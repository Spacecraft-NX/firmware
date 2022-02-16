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
#include <board.h>

void clocks_init()
{
	// FPGA, ADC (HDH), CK_OUT, USB_POWER_DETECT, LED
	rcu_periph_clock_enable(RCU_GPIOA);

	// FPGA, ADC (HAC/HAD), LIS3DH, BOARD_ID, LED
	rcu_periph_clock_enable(RCU_GPIOB);

	// FPGA
	rcu_periph_clock_enable(RCU_SPI0);

	// FPGA Sync
	rcu_periph_clock_enable(RCU_GPIOF);
}

void clock_output_init()
{
	rcu_ckout_config(RCU_CKOUTSRC_CKPLL_DIV2, 1);
	gpio_mode_set(CK_OUT_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, CK_OUT_GPIO_PIN);
	gpio_af_set(CK_OUT_GPIO_PORT, GPIO_AF_0, CK_OUT_GPIO_PIN);
}