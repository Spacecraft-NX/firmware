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
