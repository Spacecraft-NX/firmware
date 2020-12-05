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

void enter_sleep()
{
	leds_set_color(0);
	fpga_power_off();
	while ( 1 )
		pmu_to_standbymode(WFI_CMD);
}

void firmware_main()
{
	delay_init(96);

	clocks_init();
	leds_init();
	clock_output_init();
	fpga_init();
	board_id_init();

	adc_init(CONSOLE_STATE_ADC_PORT, CONSOLE_STATE_ADC_PIN, 3);
	uint16_t pa3_voltage = adc_wait_eoc_read();

	if (fpga_reset() < 0)
	{
		leds_set_color(0x3f0000);
		while (1);
	}
	else if (pa3_voltage < 1496)
	{
		config cfg;
		if (config_load(&cfg) == 0xBAD0010B)
		{
			leds_set_training(1);
			int trains_left = 100;
			while (trains_left)
			{
				uint32_t status = glitch(&null_logger);
				if (status == 0x900D0006)
					trains_left--;
				if (status == 0xBAD00107)
					break;
			}
			leds_set_training(0); 
		}

		uint32_t status = glitch(&null_logger);

		if (status == 0x900D0006)
			sdio_handler();
		else
			delay_ms(5000);
	}
	enter_sleep();
}
