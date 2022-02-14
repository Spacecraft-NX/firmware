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
#include <delay.h>

void delay_init()
{
	SysTick->LOAD = 0xFFFFFF;
	SysTick->VAL = 0;
	SysTick->CTRL = 5;
}

void SysTick_delay(uint64_t val)
{
	uint32_t i = SysTick->VAL;
	while (1)
	{
		uint32_t new_val = SysTick->VAL;
		uint32_t diff = (i - new_val) & 0xFFFFFF;
		if (diff >= val)
			break;
		val -= diff;
		i = new_val;
	}
}

void delay_ms(uint32_t nms)
{
	SysTick_delay((uint64_t)96000 * (uint64_t)nms);
}

void delay_us(uint32_t nus)
{
	SysTick_delay((uint64_t)96 * (uint64_t)nus);
}
