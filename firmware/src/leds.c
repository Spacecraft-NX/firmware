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
#include <leds.h>
#include <board.h>

#define RED_TIMER TIMER15
#define GREEN_TIMER TIMER16
#define BLUE_TIMER TIMER0
#define PULSE_TIMER TIMER13

void leds_init()
{
	gpio_af_set(LED_RED_GPIO_PORT, GPIO_AF_2, LED_RED_GPIO_PIN);
	gpio_af_set(LED_GREEN_GPIO_PORT, GPIO_AF_2, LED_GREEN_GPIO_PIN);
	gpio_af_set(LED_BLUE_GPIO_PORT, GPIO_AF_2, LED_BLUE_GPIO_PIN);
	gpio_output_options_set(LED_RED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_RED_GPIO_PIN);
	gpio_output_options_set(LED_GREEN_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_GREEN_GPIO_PIN);
	gpio_output_options_set(LED_BLUE_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_BLUE_GPIO_PIN);
	gpio_mode_set(LED_RED_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_RED_GPIO_PIN);
	gpio_mode_set(LED_GREEN_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_GREEN_GPIO_PIN); 
	gpio_mode_set(LED_BLUE_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_BLUE_GPIO_PIN);

	timer_deinit(BLUE_TIMER);
	timer_deinit(RED_TIMER);
	timer_deinit(GREEN_TIMER);
	timer_parameter_struct initpara;
	initpara.prescaler = 0x6B;
	initpara.alignedmode = TIMER_COUNTER_EDGE;
	initpara.counterdirection = TIMER_COUNTER_UP;
	initpara.period = 0x3FFF;
	initpara.clockdivision = TIMER_CKDIV_DIV1;
	initpara.repetitioncounter = 0;
	timer_init(BLUE_TIMER, &initpara);
	timer_init(RED_TIMER, &initpara);
	timer_init(GREEN_TIMER, &initpara);
	timer_oc_parameter_struct ocpara;
	ocpara.outputstate = TIMER_CCX_ENABLE;
	ocpara.outputnstate = TIMER_CCXN_ENABLE;
	ocpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
	ocpara.ocnpolarity = TIMER_OCN_POLARITY_LOW;
	ocpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
	ocpara.ocnidlestate = TIMER_OCN_IDLE_STATE_HIGH;
	timer_channel_output_config(RED_TIMER, TIMER_CH_0, &ocpara);
	timer_channel_output_pulse_value_config(RED_TIMER, TIMER_CH_0, 0x100);
	timer_channel_output_mode_config(RED_TIMER, TIMER_CH_0, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(RED_TIMER, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
	timer_primary_output_config(RED_TIMER, ENABLE);
	timer_auto_reload_shadow_enable(RED_TIMER);
	timer_channel_output_config(GREEN_TIMER, TIMER_CH_0, &ocpara);
	timer_channel_output_pulse_value_config(GREEN_TIMER, TIMER_CH_0, 0x100);
	timer_channel_output_mode_config(GREEN_TIMER, TIMER_CH_0, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(GREEN_TIMER, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
	timer_primary_output_config(GREEN_TIMER, ENABLE);
	timer_auto_reload_shadow_enable(GREEN_TIMER);
	timer_channel_output_config(BLUE_TIMER, TIMER_CH_2, &ocpara);
	timer_channel_output_pulse_value_config(BLUE_TIMER, TIMER_CH_2, 0x10000);
	timer_channel_output_mode_config(BLUE_TIMER, TIMER_CH_2, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(BLUE_TIMER, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);
	timer_primary_output_config(BLUE_TIMER, ENABLE);
	timer_auto_reload_shadow_enable(BLUE_TIMER);
	timer_enable(BLUE_TIMER);
	timer_enable(GREEN_TIMER);
	timer_enable(RED_TIMER);
	nvic_irq_enable(TIMER13_IRQn, 1, 1);
}

static void led_set_red_color(uint8_t value)
{
	timer_channel_output_pulse_value_config(RED_TIMER, 0, 0x4000 - ((0x4000 * value) / 0xFF));
}

void led_set_green_color(uint8_t value)
{
	timer_channel_output_pulse_value_config(GREEN_TIMER, 0, 0x4000 - ((0x4000 * value) / 0xFF));
}

static void led_set_blue_color(uint8_t value)
{
	timer_channel_output_pulse_value_config(BLUE_TIMER, 2, 0x4000 - ((0x4000 * value) / 0xFF));
}

void leds_set_color(uint32_t rgb)
{
	led_set_red_color(rgb >> 16);
	led_set_green_color(rgb >> 8);
	led_set_blue_color(rgb);
}

static uint8_t is_pulsing;
static uint8_t pulse_overrun;
static uint32_t blue_color_pulse;
static uint8_t is_training_mode;

void leds_set_pulsing(int enabled)
{
	timer_parameter_struct init_para; // [sp+0h] [bp-20h]

	if (is_pulsing != enabled)
	{
		is_pulsing = enabled;
		pulse_overrun = 0;
		blue_color_pulse = 0;
		timer_deinit(PULSE_TIMER);
		if (enabled)
		{
			init_para.prescaler = 7;
			init_para.alignedmode = TIMER_COUNTER_EDGE;
			init_para.counterdirection = TIMER_COUNTER_UP;
			init_para.clockdivision = TIMER_CKDIV_DIV1;
			init_para.repetitioncounter = 0;
			init_para.period = 0xFF;
			timer_init(PULSE_TIMER, &init_para);
			timer_update_event_enable(PULSE_TIMER);
			timer_interrupt_enable(PULSE_TIMER, 1);
			timer_interrupt_flag_clear(PULSE_TIMER, 1);
			timer_update_source_config(PULSE_TIMER, 1);
			timer_enable(PULSE_TIMER);
		}
	}
}

void leds_set_training(int enabled)
{
	is_training_mode = enabled;
}

void TIMER13_IRQHandler()
{
	if (timer_interrupt_flag_get(PULSE_TIMER, 1))
	{
		timer_interrupt_flag_clear(PULSE_TIMER, 1);
		if (is_pulsing == 1)
		{
			if (pulse_overrun)
			{
				if (!--blue_color_pulse)
					pulse_overrun = 0;
			}
			else if (++blue_color_pulse == 0x4000)
			{
				pulse_overrun = 1;
			}

			if (!is_training_mode)
			{
				timer_channel_output_pulse_value_config(RED_TIMER, 0, blue_color_pulse);
				timer_channel_output_pulse_value_config(GREEN_TIMER, 0, 0x4000);
				timer_channel_output_pulse_value_config(BLUE_TIMER, 2, blue_color_pulse);
			}
			else
			{
				timer_channel_output_pulse_value_config(RED_TIMER, 0, blue_color_pulse);
				timer_channel_output_pulse_value_config(GREEN_TIMER, 0, blue_color_pulse);
				timer_channel_output_pulse_value_config(BLUE_TIMER, 2, 0x4000);
			}
		}
	}
}