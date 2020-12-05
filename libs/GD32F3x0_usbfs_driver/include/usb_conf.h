/*
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

#ifndef USB_CONF_H
#define USB_CONF_H

#include "gd32f3x0.h"

#define USBFS_CORE

#ifdef USE_USBHS
	#define USBHS_CORE
#endif /* USE_USBHS */

#ifdef USBFS_CORE
	#define RX_FIFO_FS_SIZE						 128
	#define TX0_FIFO_FS_SIZE						64
	#define TX1_FIFO_FS_SIZE						128
	#define TX2_FIFO_FS_SIZE						0
	#define TX3_FIFO_FS_SIZE						0

// #define USBFS_LOW_PWR_MGMT_SUPPORT
// #define USBFS_SOF_OUTPUT_ENABLED
#endif /* USBFS_CORE */

#ifdef USBHS_CORE
	#define RX_FIFO_HS_SIZE						  512
	#define TX0_FIFO_HS_SIZE						 128
	#define TX1_FIFO_HS_SIZE						 372
	#define TX2_FIFO_HS_SIZE						 0
	#define TX3_FIFO_HS_SIZE						 0
	#define TX4_FIFO_HS_SIZE						 0
	#define TX5_FIFO_HS_SIZE						 0

	#ifdef USE_ULPI_PHY
		#define USB_ULPI_PHY_ENABLED
	#endif

	#ifdef USE_EMBEDDED_PHY
		#define USB_EMBEDDED_PHY_ENABLED
	#endif

//	#define USBHS_INTERNAL_DMA_ENABLED
	#define USBHS_DEDICATED_EP1_ENABLED
//	#define USBHS_LOW_PWR_MGMT_SUPPORT
//	#define USBHS_SOF_OUTPUT_ENABLED
#endif /* USBHS_CORE */

//#define VBUS_SENSING_ENABLED

//#define USE_HOST_MODE
#define USE_DEVICE_MODE
//#define USE_OTG_MODE

/* In HS mode and when the DMA is used, all variables and data structures dealing
   with the DMA during the transaction process should be 4-bytes aligned */
#ifdef USBHS_INTERNAL_DMA_ENABLED
	#if defined   (__GNUC__)			/* GNU Compiler */
		#define __ALIGN_END			 __attribute__ ((aligned (4)))
		#define __ALIGN_BEGIN
	#else
		#define __ALIGN_END

		#if defined   (__CC_ARM)		/* ARM Compiler */
			#define __ALIGN_BEGIN	   __align(4)
		#elif defined (__ICCARM__)	  /* IAR Compiler */
			#define __ALIGN_BEGIN 
		#elif defined  (__TASKING__)	/* TASKING Compiler */
			#define __ALIGN_BEGIN	   __align(4) 
		#endif /* __CC_ARM */  
	#endif /* __GNUC__ */ 
#else
	#define __ALIGN_BEGIN
	#define __ALIGN_END
#endif /* USBHS_INTERNAL_DMA_ENABLED */

#endif /* USB_CONF_H */
