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
#include <adc.h>
#include <fpga.h>
#include <delay.h>

void adc_init(uint32_t gpio_periph, uint32_t pin, uint8_t channel)
{
	adc_deinit();
	rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);
	rcu_periph_clock_enable(RCU_ADC);
	gpio_mode_set(gpio_periph, 3, 0, pin);
	adc_channel_length_config(1, 1);
	adc_regular_channel_config(0, channel, 0);
	adc_external_trigger_config(1, 0);
	adc_external_trigger_source_config(1, 0xE0000);
	adc_data_alignment_config(0);
	adc_resolution_config(0);
	adc_special_function_config(2, 0);
	adc_special_function_config(0x100, 0);
	adc_special_function_config(0x400, 0);
	adc_software_trigger_enable(1);
	adc_enable();
	adc_calibration_enable();
}

uint16_t adc_wait_eoc_read()
{
	ADC_CTL1 |= (uint32_t)ADC_CTL1_ADCON;

	while (!(ADC_STAT & ADC_STAT_EOC));

	return adc_regular_data_read();
}

int init_device_specific_adc(enum DEVICE_TYPE dt, struct adc_param *pap)
{
	if (dt == DEVICE_TYPE_ERISTA)
	{
		adc_init(GPIOB, GPIO_PIN_0, 8);
		pap->poweron_threshold = 1200;
		pap->glitch_power_threshold = 1376;
		return 0;
	}
	if (dt == DEVICE_TYPE_MARIKO)
	{
		adc_init(GPIOB, GPIO_PIN_1, 9);
		pap->poweron_threshold = 1024;
		pap->glitch_power_threshold = 1296;
		return 0;
	}
	if (dt == DEVICE_TYPE_LITE)
	{
		adc_init(GPIOA, GPIO_PIN_2, 2);
		pap->poweron_threshold = 1024;
		pap->glitch_power_threshold = 1296;
		return 0;
	}
	return 0xBAD00107;
}

int adc_wait_for_min_value(logger *lgr, unsigned int min_adc_value, uint16_t *adc_read_out)
{
	fpga_reset_device(0);
	uint16_t first_adc_read = adc_wait_eoc_read();
	lgr->adc(first_adc_read | 0x30000000);
	int retry = 0;
	while (1)
	{
		uint16_t adc_read = adc_wait_eoc_read();
		if (adc_read_out)
			*adc_read_out = adc_read;
		if (adc_read >= min_adc_value)
		{
			lgr->adc(adc_read | 0x10000000);
			return 0;
		}
		++retry;
		delay_us(500);
		if (retry == 2000)
		{
			lgr->adc(adc_read | 0x20000000);
			return 0xBAD00122;
		}
		if ((retry << 26) == 0)
			lgr->adc(adc_read);
	}
}
