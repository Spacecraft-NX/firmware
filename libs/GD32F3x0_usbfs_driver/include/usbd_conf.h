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

#ifndef USBD_CONF_H
#define USBD_CONF_H

#include "usb_conf.h"

#define USBD_CFG_MAX_NUM				   1U
#define USBD_ITF_MAX_NUM				   1U
#define USB_STR_DESC_MAX_SIZE			  64U

/* USB feature -- Self Powered */
#define USBD_SELF_POWERED

#define USB_STRING_COUNT				   4

/* endpoint count used by the CDC ACM device */
#define CDC_ACM_CMD_EP					 EP2_IN
#define CDC_ACM_DATA_IN_EP				 EP1_IN
#define CDC_ACM_DATA_OUT_EP				EP3_OUT

#define CDC_ACM_CMD_PACKET_SIZE			8

#define CDC_ACM_DATA_PACKET_SIZE		   64

#endif /* USBD_CONF_H */
