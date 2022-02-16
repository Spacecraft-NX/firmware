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

#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>
#include <device.h>
#include <logger.h>

void adc_init(uint32_t gpio_periph, uint32_t pin, uint8_t channel);

uint16_t adc_wait_eoc_read();

struct adc_param
{
	uint16_t poweron_threshold; // min ADC value where device is considered on (~0.8V)
	uint16_t glitch_threshold; // min ADC value from where we can begin attemping to glitch (~1.2V)
};

int init_device_specific_adc(enum DEVICE_TYPE dt, struct adc_param *pap);
int adc_wait_for_min_value(logger *lgr, unsigned int min_adc_value, uint16_t *adc_read_out);

#endif