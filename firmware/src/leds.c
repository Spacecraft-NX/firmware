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

#define OFF     0x00, 0x00, 0x00
#define RED     0x3F, 0x00, 0x00
#define GREEN   0x00, 0x3F, 0x00
#define BLUE    0x00, 0x00, 0x3F
#define ORANGE  0x3F, 0x18, 0x00
#define YELLOW  0x30, 0x3F, 0x00
#define CYAN    0x00, 0x3F, 0x3F
#define PURPLE  0x3F, 0x00, 0x3F
#define WHITE   0x3F, 0x3F, 0x3F

led_pattern_t led_state = {solid, OFF};
led_pattern_t led_delayed_state;
led_pattern_t led_override_state;
volatile uint32_t led_apply_delay = 0;
volatile uint32_t led_override_duration = 0;
volatile uint16_t led_counter = 0;

led_pattern_t lp_glitch_prepare   = {blink, PURPLE};
led_pattern_t lp_glitch_glitching = {pulse, PURPLE};
led_pattern_t lp_glitch_done      = {solid, GREEN};
led_pattern_t lp_train_prepare    = {blink, YELLOW};
led_pattern_t lp_train_glitching  = {pulse, YELLOW};
led_pattern_t lp_train_done       = {solid, YELLOW};
led_pattern_t lp_usb              = {pulse, BLUE};
led_pattern_t lp_toolbox          = {pulse, GREEN};
led_pattern_t lp_fw_write         = {blink, RED};
led_pattern_t lp_fw_read          = {blink, ORANGE};
led_pattern_t lp_flash_payload    = {blink, ORANGE};
led_pattern_t lp_err_emmc         = {solid, RED};
led_pattern_t lp_err_exhausted    = {pulse, RED};
led_pattern_t lp_err_adc_timeout  = {blink, CYAN};
led_pattern_t lp_err_adc          = {solid, WHITE};
led_pattern_t lp_err_fpga         = {solid, CYAN};
led_pattern_t lp_err_glitch       = {solid, PURPLE};
led_pattern_t lp_err_firmware     = {solid, ORANGE};
led_pattern_t lp_err_unknown      = {solid, ORANGE};
led_pattern_t lp_config_reset     = {solid, BLUE};
led_pattern_t lp_off              = {solid, 0, 0, 0};


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

	rcu_periph_clock_enable(RCU_TIMER15); // red
	rcu_periph_clock_enable(RCU_TIMER16); // green
	rcu_periph_clock_enable(RCU_TIMER0);  // blue
	rcu_periph_clock_enable(RCU_TIMER13); // pulse/blink/delay

	timer_deinit(BLUE_TIMER);
	timer_deinit(PULSE_TIMER);

	timer_parameter_struct initpara;
	initpara.prescaler = 0x6B;
	initpara.alignedmode = TIMER_COUNTER_EDGE;
	initpara.counterdirection = TIMER_COUNTER_UP;
	initpara.period = 0x3FFF;
	initpara.clockdivision = TIMER_CKDIV_DIV1;
	initpara.repetitioncounter = 0;
	timer_init(RED_TIMER, &initpara);
	timer_init(GREEN_TIMER, &initpara);
	timer_init(BLUE_TIMER, &initpara);

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

	// configure timer to 1.024KHz
	initpara.prescaler = 49;
	initpara.alignedmode = TIMER_COUNTER_EDGE;
	initpara.counterdirection = TIMER_COUNTER_UP;
	initpara.clockdivision = TIMER_CKDIV_DIV1;
	initpara.repetitioncounter = 0;
	initpara.period = 1874;
	timer_init(PULSE_TIMER, &initpara);
	timer_update_event_enable(PULSE_TIMER);
	timer_interrupt_enable(PULSE_TIMER, 1);
	timer_update_source_config(PULSE_TIMER, 1);

	timer_channel_output_pulse_value_config(RED_TIMER, 0, 0x4000);
	timer_channel_output_pulse_value_config(GREEN_TIMER, 0, 0x4000);
	timer_channel_output_pulse_value_config(BLUE_TIMER, 2, 0x4000);

	timer_enable(BLUE_TIMER);
	timer_enable(GREEN_TIMER);
	timer_enable(RED_TIMER);
	timer_enable(PULSE_TIMER);

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

led_pattern_t leds_get_pattern()
{
	return led_state;
}

void leds_set_pattern(const led_pattern_t *pattern)
{
	// reset blink/pulse counter only when pattern changes
	if (led_state.type != pattern->type || led_state.red != pattern->red ||
		led_state.green != pattern->green || led_state.blue != pattern->blue)
	{
		led_counter = 0;
	}

	led_state = *pattern;
	led_apply_delay = 0; // cancel outstanding delayed pattern applications
}

void leds_set_pattern_delayed(const led_pattern_t *pattern, int delay_ms)
{
	led_delayed_state = *pattern;
	led_apply_delay = delay_ms; // cancel outstanding delayed pattern applications
}

void leds_override(uint32_t duration_ms, const led_pattern_t *pattern)
{
	led_override_state = *pattern;
	led_override_duration = duration_ms;
}

void leds_off()
{
	timer_disable(BLUE_TIMER);
	timer_disable(GREEN_TIMER);
	timer_disable(RED_TIMER);
	timer_disable(PULSE_TIMER);

	rcu_periph_clock_disable(RCU_TIMER15); // red
	rcu_periph_clock_disable(RCU_TIMER16); // green
	rcu_periph_clock_disable(RCU_TIMER0);  // blue
	rcu_periph_clock_disable(RCU_TIMER13); // pulse/blink/delay

	nvic_irq_disable(TIMER13_IRQn);
}

void apply_led_pattern(const led_pattern_t *pattern)
{
	if (pattern->type == solid)
	{
		led_set_red_color(pattern->red);
		led_set_green_color(pattern->green);
		led_set_blue_color(pattern->blue);
	}
	else if (pattern->type == blink)
	{
		led_set_red_color(led_counter < 64 ? pattern->red : 0);
		led_set_green_color(led_counter < 64 ? pattern->green : 0);
		led_set_blue_color(led_counter < 64 ? pattern->blue : 0);
		led_counter &= 0xFF; // reduce to 4Hz
	}
	else // pulse
	{
		uint32_t val = 0x4000 - (((led_counter < 512) ? led_counter : (1024 - led_counter)) << 5);
		timer_channel_output_pulse_value_config(RED_TIMER, 0, pattern->red ? val : 0x4000);
		timer_channel_output_pulse_value_config(GREEN_TIMER, 0, pattern->green ? val : 0x4000);
		timer_channel_output_pulse_value_config(BLUE_TIMER, 2, pattern->blue ? val : 0x4000);
	}
}

void TIMER13_IRQHandler()
{
	timer_interrupt_flag_clear(PULSE_TIMER, 1);

	if (led_apply_delay > 0)
	{
		led_apply_delay--;
		if (!led_apply_delay)
		{
			leds_set_pattern(&led_delayed_state);
		}
	}

	// Check override
	if (led_override_duration > 0)
	{
		--led_override_duration;
		apply_led_pattern(&led_override_state);
	}
	else
	{
		apply_led_pattern(&led_state);
	}

	led_counter++;
	led_counter &= 0x3ff; // overflow at 1Hz
}