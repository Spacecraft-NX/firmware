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

#include "gd32f3x0_fwdgt.h"

/*!
	\brief	  enable write access to FWDGT_PSC and FWDGT_RLD
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void fwdgt_write_enable(void)
{
	FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;
}

/*!
	\brief	  disable write access to FWDGT_PSC,FWDGT_RLD and FWDGT_WND
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void fwdgt_write_disable(void)
{
	FWDGT_CTL = FWDGT_WRITEACCESS_DISABLE;
}

/*!
	\brief	  start the free watchdog timer counter
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void fwdgt_enable(void)
{
	FWDGT_CTL = FWDGT_KEY_ENABLE;
}

/*!
	\brief	  configure the free watchdog timer counter window value
	\param[in]  window_value: specify window value(0x0000 - 0x0FFF)
	\param[out] none
	\retval	 ErrStatus: ERROR or SUCCESS
*/
ErrStatus fwdgt_window_value_config(uint16_t window_value)
{
	uint32_t time_index = FWDGT_WND_TIMEOUT;
	uint32_t flag_status = RESET;

	/* enable write access to FWDGT_WND */
	FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;

	/* wait until the WUD flag to be reset */
	do{
		flag_status = FWDGT_STAT & FWDGT_STAT_WUD;
	}while((--time_index > 0U) && (RESET != flag_status));

	if (RESET != flag_status){
		return ERROR; 
	}
	
	FWDGT_WND = WND_WND(window_value);

	return SUCCESS;
}

/*!
	\brief	  reload the counter of FWDGT
	\param[in]  none
	\param[out] none
	\retval	 none
*/
void fwdgt_counter_reload(void)
{
	FWDGT_CTL = FWDGT_KEY_RELOAD;
}


/*!
	\brief	  configure counter reload value, and prescaler divider value
	\param[in]  reload_value: specify reload value(0x0000 - 0x0FFF)
	\param[in]  prescaler_div: FWDGT prescaler value
				only one parameter can be selected which is shown as below:
	  \arg		FWDGT_PSC_DIV4: FWDGT prescaler set to 4
	  \arg		FWDGT_PSC_DIV8: FWDGT prescaler set to 8
	  \arg		FWDGT_PSC_DIV16: FWDGT prescaler set to 16
	  \arg		FWDGT_PSC_DIV32: FWDGT prescaler set to 32
	  \arg		FWDGT_PSC_DIV64: FWDGT prescaler set to 64
	  \arg		FWDGT_PSC_DIV128: FWDGT prescaler set to 128
	  \arg		FWDGT_PSC_DIV256: FWDGT prescaler set to 256
	\param[out] none
	\retval	 ErrStatus: ERROR or SUCCESS
*/
ErrStatus fwdgt_config(uint16_t reload_value, uint8_t prescaler_div)
{
	uint32_t timeout = FWDGT_PSC_TIMEOUT;
	uint32_t flag_status = RESET;
  
	/* enable write access to FWDGT_PSC,and FWDGT_RLD */
	FWDGT_CTL = FWDGT_WRITEACCESS_ENABLE;
  
	/* wait until the PUD flag to be reset */
	do{
		flag_status = FWDGT_STAT & FWDGT_STAT_PUD;
	}while((--timeout > 0U) && (RESET != flag_status));
	
	if (RESET != flag_status){
		return ERROR;
	}
	
	/* configure FWDGT */
	FWDGT_PSC = (uint32_t)prescaler_div;	   

	timeout = FWDGT_RLD_TIMEOUT;	
	/* wait until the RUD flag to be reset */
	do{
		flag_status = FWDGT_STAT & FWDGT_STAT_RUD;
	}while((--timeout > 0U) && (RESET != flag_status));
   
	if (RESET != flag_status){
		return ERROR;
	}
	
	FWDGT_RLD = RLD_RLD(reload_value);
	
	/* reload the counter */
	FWDGT_CTL = FWDGT_KEY_RELOAD;
	
	return SUCCESS;
}

/*!
	\brief	  get flag state of FWDGT
	\param[in]  flag: flag to get
				only one parameter can be selected which is shown as below:
	  \arg		FWDGT_FLAG_PUD: a write operation to FWDGT_PSC register is on going
	  \arg		FWDGT_FLAG_RUD: a write operation to FWDGT_RLD register is on going
	  \arg		FWDGT_FLAG_WUD: a write operation to FWDGT_WND register is on going
	\param[out] none
	\retval	 FlagStatus: SET or RESET
*/
FlagStatus fwdgt_flag_get(uint16_t flag)
{
  if(FWDGT_STAT & flag){
		return SET;
  }
	return RESET;
}
