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

#ifndef GD32F3X0_LIBOPT_H
#define GD32F3X0_LIBOPT_H

#include "gd32f3x0_adc.h"
#include "gd32f3x0_crc.h"
#include "gd32f3x0_ctc.h"
#include "gd32f3x0_dbg.h"
#include "gd32f3x0_dma.h"
#include "gd32f3x0_exti.h"
#include "gd32f3x0_fmc.h"
#include "gd32f3x0_gpio.h"
#include "gd32f3x0_syscfg.h"
#include "gd32f3x0_i2c.h"
#include "gd32f3x0_fwdgt.h"
#include "gd32f3x0_pmu.h"
#include "gd32f3x0_rcu.h"
#include "gd32f3x0_rtc.h"
#include "gd32f3x0_spi.h"
#include "gd32f3x0_timer.h"
#include "gd32f3x0_usart.h"
#include "gd32f3x0_wwdgt.h"
#include "gd32f3x0_misc.h"
#include "gd32f3x0_tsi.h"

#ifdef GD32F350
#include "gd32f3x0_cec.h"
#include "gd32f3x0_cmp.h"
#include "gd32f3x0_dac.h"
#endif /* GD32F350 */

#endif /* GD32F3X0_LIBOPT_H */
