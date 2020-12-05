/*
 * Copyright (c) 2019 GigaDevice Semiconductor Inc.
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

#include "gd32f3x0_wwdgt.h"
#include "gd32f3x0_rcu.h"

/*!
	\brief	  reset the window watchdog timer configuration
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void wwdgt_deinit(void)
{
	rcu_periph_reset_enable(RCU_WWDGTRST);
	rcu_periph_reset_disable(RCU_WWDGTRST);
}

/*!
	\brief	  start the window watchdog timer counter
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void wwdgt_enable(void)
{
	WWDGT_CTL |= WWDGT_CTL_WDGTEN;
}

/*!
	\brief	  configure the window watchdog timer counter value
	\param[in]  counter_value: 0x00 - 0x7F
	\param[out] none
	\retval	 none
*/
void wwdgt_counter_update(uint16_t counter_value)
{
	uint32_t reg = 0x0U;
	
	reg = WWDGT_CTL &(~(uint32_t)WWDGT_CTL_CNT);
	reg |= (uint32_t)(CTL_CNT(counter_value));
	
	WWDGT_CTL = (uint32_t)reg;
}

/*!
	\brief	  configure counter value, window value, and prescaler divider value  
	\param[in]  counter: 0x00 - 0x7F   
	\param[in]  window: 0x00 - 0x7F
	\param[in]  prescaler: wwdgt prescaler value
				only one parameter can be selected which is shown as below:
	  \arg		WWDGT_CFG_PSC_DIV1: the time base of window watchdog counter = (PCLK1/4096)/1
	  \arg		WWDGT_CFG_PSC_DIV2: the time base of window watchdog counter = (PCLK1/4096)/2
	  \arg		WWDGT_CFG_PSC_DIV4: the time base of window watchdog counter = (PCLK1/4096)/4
	  \arg		WWDGT_CFG_PSC_DIV8: the time base of window watchdog counter = (PCLK1/4096)/8
	\param[out] none
	\retval	 none
*/
void wwdgt_config(uint16_t counter, uint16_t window, uint32_t prescaler)
{
	uint32_t reg_cfg = 0x0U, reg_ctl = 0x0U;

	/* clear WIN and PSC bits, clear CNT bit */
	reg_cfg = WWDGT_CFG &(~((uint32_t)WWDGT_CFG_WIN|(uint32_t)WWDGT_CFG_PSC));
	reg_ctl = WWDGT_CTL &(~(uint32_t)WWDGT_CTL_CNT);
  
	/* configure WIN and PSC bits, configure CNT bit */
	reg_cfg |= (uint32_t)(CFG_WIN(window));
	reg_cfg |= (uint32_t)(prescaler);
	reg_ctl |= (uint32_t)(CTL_CNT(counter));
	
	WWDGT_CFG = (uint32_t)reg_cfg;
	WWDGT_CTL = (uint32_t)reg_ctl;
}

/*!
	\brief	  enable early wakeup interrupt of WWDGT
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void wwdgt_interrupt_enable(void)
{
	WWDGT_CFG |= WWDGT_CFG_EWIE;
}

/*!
	\brief	  check early wakeup interrupt state of WWDGT
	\param[in]  none
	\param[out] none
	\retval	 FlagStatus: SET or RESET
*/
FlagStatus wwdgt_flag_get(void)
{
  if(WWDGT_STAT & WWDGT_STAT_EWIF){
		return SET;
  }
	return RESET;
}

/*!
	\brief	  clear early wakeup interrupt state of WWDGT
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void wwdgt_flag_clear(void)
{
	WWDGT_STAT &= (uint32_t)(~(uint32_t)WWDGT_STAT_EWIF);
}
