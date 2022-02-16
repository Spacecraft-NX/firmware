/*
 * Copyright (c) 2020 Spacecraft-NX
 * Copyright (c) 2017 GigaDevice Semiconductor Inc.
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

#include <gd32f3x0_it.h>
#include <usbd_int.h>
#include <leds.h>

extern usb_core_handle_struct usbfs_core_dev;
extern uint32_t usbfs_prescaler;

/*!
	\brief	  this function handles NMI exception
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void NMI_Handler(void)
{
}

/*!
	\brief	  this function handles HardFault exception
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void HardFault_Handler(void)
{
	/* if Hard Fault exception occurs, go to infinite loop */
	leds_set_pattern(&lp_err_firmware);
	while (1);
}

/*!
	\brief	  this function handles MemManage exception
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void MemManage_Handler(void)
{
	/* if Memory Manage exception occurs, go to infinite loop */
	while (1);
}

/*!
	\brief	  this function handles BusFault exception
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void BusFault_Handler(void)
{
	/* if Bus Fault exception occurs, go to infinite loop */
	while (1);
}

/*!
	\brief	  this function handles UsageFault exception
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void UsageFault_Handler(void)
{
	/* if Usage Fault exception occurs, go to infinite loop */
	leds_set_pattern(&lp_err_firmware);
	while (1);
}

/*!
	\brief	  this function handles SVC exception
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void SVC_Handler(void)
{
}

/*!
	\brief	  this function handles DebugMon exception
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void DebugMon_Handler(void)
{
}

/*!
	\brief	  this function handles PendSV exception
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void PendSV_Handler(void)
{
}

/*!
	\brief	  this function handles SysTick exception
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void SysTick_Handler(void)
{
}

/*!
	\brief	  this function handles USBD interrupt
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void  USBFS_IRQHandler (void)
{
	usbd_isr (&usbfs_core_dev);
}

/*!
	\brief	  this function handles USBD wakeup interrupt request.
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void USBFS_WKUP_IRQHandler(void)
{
	if (usbfs_core_dev.cfg.low_power) {
		SystemInit();

		rcu_usbfs_clock_config(RCU_USBFS_CKPLL_DIV2);

		rcu_periph_clock_enable(RCU_USBFS);

		usb_clock_ungate(&usbfs_core_dev);
	}

	exti_interrupt_flag_clear(EXTI_18);
}
