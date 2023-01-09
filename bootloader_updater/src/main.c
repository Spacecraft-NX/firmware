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
#include <cdc_acm_core.h>
#include <bootloader.h>
#include <dfu.h>

usb_core_handle_struct usbfs_core_dev =
{
	.dev = 
	{
		.dev_desc = (uint8_t *)&device_descriptor,
		.config_desc = (uint8_t *)&configuration_descriptor,
		.strings = usbd_strings,
		.class_init = cdc_acm_init,
		.class_deinit = cdc_acm_deinit,
		.class_req_handler = cdc_acm_req_handler,
		.class_data_handler = cdc_acm_data_handler
	},

	.udelay = delay_us,
	.mdelay = delay_ms
};

int usb_receive_data()
{
	receive_length = 0;
	cdc_acm_data_receive(&usbfs_core_dev);
	while (!packet_receive);
	return receive_length;
}

void usb_send_data(int len)
{
	packet_sent = 0;
	cdc_acm_data_send(&usbfs_core_dev, len);
	while (!packet_sent);
	
	// We need to send a ZLP to signal end of bulk in case the data length is multiple of max packet size
	// Due our buffer size is 64 this only happens in the case we send 64 bytes.
	if (len == 64)
		usb_send_data(0);
}

void usb_wait_till_ready()
{
	while (usbfs_core_dev.dev.status != USB_STATUS_CONFIGURED);
}

struct bootloader_usb g_bootloader_usb =
{
	.receive_buffer = usb_recv_data_buffer,
	.send_buffer = usb_send_data_buffer,
	.receive_data = usb_receive_data,
	.send_data = usb_send_data,
	.wait_till_ready = usb_wait_till_ready,
};

void usb_interrupt_config(void)
{
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	nvic_irq_enable((uint8_t)USBFS_IRQn, 2U, 0U);

	/* enable the power module clock */
	rcu_periph_clock_enable(RCU_PMU);

	/* USB wakeup EXTI line configuration */
	exti_interrupt_flag_clear(EXTI_18);
	exti_init(EXTI_18, EXTI_INTERRUPT, EXTI_TRIG_RISING);
	exti_interrupt_enable(EXTI_18);

	nvic_irq_enable((uint8_t)USBFS_WKUP_IRQn, 1U, 0U);
}

void init_leds()
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_TIMER0);
	rcu_periph_clock_enable(RCU_TIMER15);
	rcu_periph_clock_enable(RCU_TIMER16);
	
	gpio_af_set(GPIOB, 2, GPIO_PIN_8);
	gpio_af_set(GPIOB, 2, GPIO_PIN_9);
	gpio_af_set(GPIOA, 2, GPIO_PIN_10);
	gpio_output_options_set(GPIOB, 0, 3, GPIO_PIN_8);
	gpio_output_options_set(GPIOB, 0, 3, GPIO_PIN_9);
	gpio_output_options_set(GPIOA, 0, 3, GPIO_PIN_10);
	gpio_mode_set(GPIOB, 2, 0, GPIO_PIN_8);
	gpio_mode_set(GPIOB, 2, 0, GPIO_PIN_9);
	gpio_mode_set(GPIOA, 2, 0, GPIO_PIN_10);
	timer_deinit(TIMER0);
	timer_deinit(TIMER15);
	timer_deinit(TIMER16);
	timer_parameter_struct initpara;
	initpara.prescaler = 107;
	initpara.alignedmode = TIMER_COUNTER_EDGE;
	initpara.counterdirection = TIMER_COUNTER_UP;
	initpara.period = 255;
	initpara.clockdivision = TIMER_CKDIV_DIV1;
	initpara.repetitioncounter = 0;
	timer_init(TIMER0, &initpara);
	timer_init(TIMER15, &initpara);
	timer_init(TIMER16, &initpara);
	timer_oc_parameter_struct v0;
	v0.outputstate = TIMER_CCX_ENABLE;
	v0.outputnstate = TIMER_CCXN_ENABLE;
	v0.ocpolarity = TIMER_OC_POLARITY_HIGH;
	v0.ocnpolarity = TIMER_OCN_POLARITY_LOW;
	v0.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
	v0.ocnidlestate = TIMER_OCN_IDLE_STATE_HIGH;
	timer_channel_output_config(TIMER15, 0, &v0);
	timer_channel_output_pulse_value_config(TIMER15, 0, 256);
	timer_channel_output_mode_config(TIMER15, 0, 96);
	timer_channel_output_shadow_config(TIMER15, 0, 0);
	timer_primary_output_config(TIMER15, 1);
	timer_auto_reload_shadow_enable(TIMER15);
	timer_channel_output_config(TIMER16, 0, &v0);
	timer_channel_output_pulse_value_config(TIMER16, 0, 256);
	timer_channel_output_mode_config(TIMER16, 0, 96);
	timer_channel_output_shadow_config(TIMER16, 0, 0);
	timer_primary_output_config(TIMER16, 1);
	timer_auto_reload_shadow_enable(TIMER16);
	timer_channel_output_config(TIMER0, 2, &v0);
	timer_channel_output_pulse_value_config(TIMER0, 2, 256);
	timer_channel_output_mode_config(TIMER0, 2, 96);
	timer_channel_output_shadow_config(TIMER0, 2, 0);
	timer_primary_output_config(TIMER0, 1);
	timer_auto_reload_shadow_enable(TIMER0);
	timer_enable(TIMER0);
	timer_enable(TIMER16);
	timer_enable(TIMER15);
}

void led_set_red_color(uint8_t value)
{
	timer_channel_output_pulse_value_config(TIMER15, 0, 0x100 - value);
}

void led_set_green_color(uint8_t value)
{
	timer_channel_output_pulse_value_config(TIMER16, 0, 0x100 - value);
}

void led_set_blue_color(uint8_t value)
{
	timer_channel_output_pulse_value_config(TIMER0, 2, 0x100 - value);
}

void leds_set_color(uint32_t rgb)
{
	led_set_red_color(rgb >> 16);
	led_set_green_color(rgb >> 8);
	led_set_blue_color(rgb);
}

int main(void)
{
	timer_interrupt_flag_clear(TIMER13, 1);
	nvic_irq_disable(TIMER13_IRQn);
	
	init_leds();
	delay_init(96);
	
	leds_set_color(0xFFFFFF);

	rcu_usbfs_clock_config(RCU_USBFS_CKPLL_DIV2);
	rcu_periph_clock_enable(RCU_USBFS);

	/* USB device stack configure */
	usbd_init(&usbfs_core_dev, USB_FS_CORE_ID);

	/* USB interrupt configure */
	usb_interrupt_config();
	
	/* check if USB device is enumerated successfully */
	while (usbfs_core_dev.dev.status != USB_STATUS_CONFIGURED) {}
	
	/* delay 10 ms */
	if(NULL != usbfs_core_dev.mdelay){
			usbfs_core_dev.mdelay(10);
	}
	
	dfu(&g_bootloader_usb);
}
