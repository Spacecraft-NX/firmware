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
#include <board.h>
#include <leds.h>
#include <fpga.h>
#include <board_id.h>
#include <adc.h>
#include <glitch.h>
#include <clock.h>
#include <sdio.h>
#include <timer.h>
#include <session_info.h>
#include <stddef.h>

void systick_irq_config(void)
{
	SysTick->CTRL |= (1UL << 1);
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	nvic_irq_enable((uint8_t)SysTick_IRQn, 1U, 0U);
}

void systick_irq_disable(void)
{
	SysTick->CTRL &= ~(1UL << 1);
	nvic_irq_disable((uint8_t)SysTick_IRQn);
}

void enter_sleep()
{
	systick_irq_disable();
	fpga_power_off();
	leds_set_color(0);
	while ( 1 )
		pmu_to_standbymode(WFI_CMD);
}

void firmware_main()
{
	delay_init();
	systick_irq_config();
	timer_global_init();
	clocks_init();
	leds_init();
	clock_output_init();
	fpga_init();
	board_id_init();

	int syncAttempt = 100;
	while (1)
	{
		gpio_mode_set(GPIOF, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BIT(7));
		delay_ms(5u);
		int pinWhilePulledUp = gpio_input_bit_get(GPIOF, BIT(7));

		// configure GPIO to input with pull-down
		gpio_mode_set(GPIOF, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, BIT(7));
		delay_ms(5u);
		int pinWhilePulledDown = gpio_input_bit_get(GPIOF, BIT(7));

		if (pinWhilePulledDown && pinWhilePulledUp && syncAttempt < 95)
		{
			fpga_sync_failed = 1;
			break;
		}
		if (pinWhilePulledUp == 1) // FPGA was driving it high
		{
			break;
		}
		if (syncAttempt == 0)
		{
			config_reset();
			leds_set_color(0x00003f); // blue
			while (1) ;
		}
		--syncAttempt;
	}

	adc_init(CONSOLE_STATE_ADC_PORT, CONSOLE_STATE_ADC_PIN, 3);
	session_info.startup_adc_value = adc_wait_eoc_read();

	if (fpga_reset() != 0x900D0000)
	{
		leds_set_color(0x3f0000);
		while (1) ;
	}
	else if (session_info.startup_adc_value < 1596)
	{
		config_t cfg;
		if (config_load(&cfg) == 0xBAD0010B)
		{
			leds_set_training(1);
			int trains_left = 50;
			uint32_t status;
			do
			{
				session_info_t si = {0};
				status = glitch(&null_logger, &si);
			} while (--trains_left && status != 0xBAD00107 && status != 0xBAD00113);
			leds_set_training(0); 
		}

		uint32_t status = glitch(&null_logger, &session_info);
		systick_irq_disable();

		if (status == 0x900D0006)
			sdio_handler();

		fpga_power_off(); // so cannot interfere with eMMC
		delay_ms(status == 0x900D0006 ? 2000 : 5000); // slightly longer LED visible for error
	}
	enter_sleep();
}
