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

#include "gd32f3x0_dbg.h"

#define DBG_RESET_VAL	   ((uint32_t)0x00000000U)   /*!< DBG reset value */

/*!
	\brief	  deinitialize the DBG
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void dbg_deinit(void)
{
	DBG_CTL0 = DBG_RESET_VAL;
	DBG_CTL1 = DBG_RESET_VAL;
}

/*!
	\brief	  read DBG_ID code register
	\param[in]  none
	\param[out] none
	\retval	 DBG_ID code
*/
uint32_t dbg_id_get(void)
{
	return DBG_ID;
}

/*!
	\brief	  enable low power behavior when the mcu is in debug mode
	\param[in]  dbg_low_power:
				one or more parameters can be selected which are shown as below:
	  \arg		DBG_LOW_POWER_SLEEP: keep debugger connection during sleep mode
	  \arg		DBG_LOW_POWER_DEEPSLEEP: keep debugger connection during deepsleep mode
	  \arg		DBG_LOW_POWER_STANDBY: keep debugger connection during standby mode
	\param[out] none
	\retval	 none
*/
void dbg_low_power_enable(uint32_t dbg_low_power)
{
	DBG_CTL0 |= dbg_low_power;
}

/*!
	\brief	  disable low power behavior when the mcu is in debug mode
	\param[in]  dbg_low_power:
				one or more parameters can be selected which are shown as below:
	  \arg		DBG_LOW_POWER_SLEEP: donot keep debugger connection during sleep mode
	  \arg		DBG_LOW_POWER_DEEPSLEEP: donot keep debugger connection during deepsleep mode
	  \arg		DBG_LOW_POWER_STANDBY: donot keep debugger connection during standby mode
	\param[out] none
	\retval	 none
*/
void dbg_low_power_disable(uint32_t dbg_low_power)
{
	DBG_CTL0 &= ~dbg_low_power;
}

/*!
	\brief	  enable peripheral behavior when the mcu is in debug mode
	\param[in]  dbg_periph: refer to dbg_periph_enum 
				one or more parameters can be selected which are shown as below:
	  \arg		DBG_SLEEP_HOLD: keep debugger connection during sleep mode
	  \arg		DBG_DEEPSLEEP_HOLD: keep debugger connection during deepsleep mode
	  \arg		DBG_STANDBY_HOLD: keep debugger connection during standby mode
	  \arg		DBG_FWDGT_HOLD: debug FWDGT kept when core is halted
	  \arg		DBG_WWDGT_HOLD: debug WWDGT kept when core is halted
	  \arg		DBG_TIMERx_HOLD (x=0,1,2,5,13,14,15,16,TIMER5 is only available in GD32F350): hold TIMERx counter when core is halted
	  \arg		DBG_I2Cx_HOLD (x=0,1): hold I2Cx smbus when core is halted
	  \arg		DBG_RTC_HOLD: hold RTC calendar and wakeup counter when core is halted
	\param[out] none
	\retval	 none
*/
void dbg_periph_enable(dbg_periph_enum dbg_periph)
{
	DBG_REG_VAL(dbg_periph) |= BIT(DBG_BIT_POS(dbg_periph));
}

/*!
	\brief	  disable peripheral behavior when the mcu is in debug mode
	\param[in]  dbg_periph: refer to dbg_periph_enum 
				one or more parameters can be selected which are shown as below:
	  \arg		DBG_SLEEP_HOLD: keep debugger connection during sleep mode
	  \arg		DBG_DEEPSLEEP_HOLD: keep debugger connection during deepsleep mode
	  \arg		DBG_STANDBY_HOLD: keep debugger connection during standby mode
	  \arg		DBG_FWDGT_HOLD: debug FWDGT kept when core is halted
	  \arg		DBG_WWDGT_HOLD: debug WWDGT kept when core is halted
	  \arg		DBG_TIMERx_HOLD (x=0,1,2,5,13,14,15,16,TIMER5 is only available in GD32F350): hold TIMERx counter when core is halted
	  \arg		DBG_I2Cx_HOLD (x=0,1): hold I2Cx smbus when core is halted
	  \arg		DBG_RTC_HOLD: hold RTC calendar and wakeup counter when core is halted
	\param[out] none
	\retval	 none
*/
void dbg_periph_disable(dbg_periph_enum dbg_periph)
{
	DBG_REG_VAL(dbg_periph) &= ~BIT(DBG_BIT_POS(dbg_periph));
}
