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

#ifndef USBD_INT_H
#define USBD_INT_H

#include "usbd_core.h"

typedef struct
{
	uint8_t (*SOF) (usb_core_handle_struct *pudev);
}usbd_int_cb_struct;

extern usbd_int_cb_struct *usbd_int_fops;

/* function declarations */
/* USB device-mode interrupts global service routine handler */
uint32_t usbd_isr (usb_core_handle_struct *pudev);

#endif /* USBD_INT_H */

