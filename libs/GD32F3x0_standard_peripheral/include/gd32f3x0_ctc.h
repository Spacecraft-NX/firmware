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

#ifndef GD32F3X0_CTC_H
#define GD32F3X0_CTC_H

#include "gd32f3x0.h"

/* CTC definitions */
#define CTC						  CTC_BASE

/* registers definitions */
#define CTC_CTL0					 REG32(CTC + 0x00000000U)  /*!< CTC control register 0 */
#define CTC_CTL1					 REG32(CTC + 0x00000004U)  /*!< CTC control register 1 */
#define CTC_STAT					 REG32(CTC + 0x00000008U)  /*!< CTC status register */
#define CTC_INTC					 REG32(CTC + 0x0000000CU)  /*!< CTC interrupt clear register */

/* bits definitions */
/* CTC_CTL0 */
#define CTC_CTL0_CKOKIE			  BIT(0)					/*!< clock trim OK(CKOKIF) interrupt enable */ 
#define CTC_CTL0_CKWARNIE			BIT(1)					/*!< clock trim warning(CKWARNIF) interrupt enable */
#define CTC_CTL0_ERRIE			   BIT(2)					/*!< error(ERRIF) interrupt enable */
#define CTC_CTL0_EREFIE			  BIT(3)					/*!< EREFIF interrupt enable */
#define CTC_CTL0_CNTEN			   BIT(5)					/*!< CTC counter enable */
#define CTC_CTL0_AUTOTRIM			BIT(6)					/*!< hardware automatically trim mode */
#define CTC_CTL0_SWREFPUL			BIT(7)					/*!< software reference source sync pulse */
#define CTC_CTL0_TRIMVALUE		   BITS(8,13)				/*!< IRC48M trim value */

/* CTC_CTL1 */
#define CTC_CTL1_RLVALUE			 BITS(0,15)				/*!< CTC counter reload value */
#define CTC_CTL1_CKLIM			   BITS(16,23)			   /*!< clock trim base limit value */
#define CTC_CTL1_REFPSC			  BITS(24,26)			   /*!< reference signal source prescaler */
#define CTC_CTL1_REFSEL			  BITS(28,29)			   /*!< reference signal source selection */
#define CTC_CTL1_REFPOL			  BIT(31)				   /*!< reference signal source polarity */

/* CTC_STAT */
#define CTC_STAT_CKOKIF			  BIT(0)					/*!< clock trim OK interrupt flag */
#define CTC_STAT_CKWARNIF			BIT(1)					/*!< clock trim warning interrupt flag */
#define CTC_STAT_ERRIF			   BIT(2)					/*!< error interrupt flag */
#define CTC_STAT_EREFIF			  BIT(3)					/*!< expect reference interrupt flag */
#define CTC_STAT_CKERR			   BIT(8)					/*!< clock trim error bit */
#define CTC_STAT_REFMISS			 BIT(9)					/*!< reference sync pulse miss */
#define CTC_STAT_TRIMERR			 BIT(10)				   /*!< trim value error bit */
#define CTC_STAT_REFDIR			  BIT(15)				   /*!< CTC trim counter direction when reference sync pulse occurred */
#define CTC_STAT_REFCAP			  BITS(16,31)			   /*!< CTC counter capture when reference sync pulse occurred */

/* CTC_INTC */
#define CTC_INTC_CKOKIC			  BIT(0)					/*!< CKOKIF interrupt clear bit */
#define CTC_INTC_CKWARNIC			BIT(1)					/*!< CKWARNIF interrupt clear bit */
#define CTC_INTC_ERRIC			   BIT(2)					/*!< ERRIF interrupt clear bit */
#define CTC_INTC_EREFIC			  BIT(3)					/*!< EREFIF interrupt clear bit */

/* constants definitions */
#define CTL0_TRIMVALUE(regval)						   (BITS(8,13) & ((uint32_t)(regval) << 8))
#define CTL1_CKLIM(regval)							   (BITS(16,23) & ((uint32_t)(regval) << 16))
#define GET_STAT_REFCAP(regval)						  GET_BITS((regval),16,31)
#define GET_CTL0_TRIMVALUE(regval)					   GET_BITS((regval),8,13)

/* hardware automatically trim mode definitions */
#define CTC_HARDWARE_TRIM_MODE_ENABLE					CTC_CTL0_AUTOTRIM			/*!< hardware automatically trim mode enable*/
#define CTC_HARDWARE_TRIM_MODE_DISABLE				   ((uint32_t)0x00000000U)	  /*!< hardware automatically trim mode disable*/

/* reference signal source polarity definitions */
#define CTC_REFSOURCE_POLARITY_FALLING				   CTC_CTL1_REFPOL			  /*!< reference signal source polarity is falling edge*/
#define CTC_REFSOURCE_POLARITY_RISING					((uint32_t)0x00000000U)	  /*!< reference signal source polarity is rising edge*/

/* reference signal source selection definitions */
#define CTL1_REFSEL(regval)							  (BITS(28,29) & ((uint32_t)(regval) << 28))
#define CTC_REFSOURCE_GPIO							   CTL1_REFSEL(0)			   /*!< GPIO is selected */
#define CTC_REFSOURCE_LXTAL							  CTL1_REFSEL(1)			   /*!< LXTAL is clock selected */
#define CTC_REFSOURCE_USBSOF							 CTL1_REFSEL(2)			   /*!< USBFSSOF selected */

/* reference signal source prescaler definitions */
#define CTL1_REFPSC(regval)							  (BITS(24,26) & ((uint32_t)(regval) << 24))
#define CTC_REFSOURCE_PSC_OFF							CTL1_REFPSC(0)			   /*!< reference signal not divided */
#define CTC_REFSOURCE_PSC_DIV2						   CTL1_REFPSC(1)			   /*!< reference signal divided by 2 */
#define CTC_REFSOURCE_PSC_DIV4						   CTL1_REFPSC(2)			   /*!< reference signal divided by 4 */
#define CTC_REFSOURCE_PSC_DIV8						   CTL1_REFPSC(3)			   /*!< reference signal divided by 8 */
#define CTC_REFSOURCE_PSC_DIV16						  CTL1_REFPSC(4)			   /*!< reference signal divided by 16 */
#define CTC_REFSOURCE_PSC_DIV32						  CTL1_REFPSC(5)			   /*!< reference signal divided by 32 */
#define CTC_REFSOURCE_PSC_DIV64						  CTL1_REFPSC(6)			   /*!< reference signal divided by 64 */
#define CTC_REFSOURCE_PSC_DIV128						 CTL1_REFPSC(7)			   /*!< reference signal divided by 128 */

/* CTC interrupt enable definitions */
#define CTC_INT_CKOK									 CTC_CTL0_CKOKIE			  /*!< clock trim OK interrupt enable */
#define CTC_INT_CKWARN								   CTC_CTL0_CKWARNIE			/*!< clock trim warning interrupt enable */
#define CTC_INT_ERR									  CTC_CTL0_ERRIE			   /*!< error interrupt enable */
#define CTC_INT_EREF									 CTC_CTL0_EREFIE			  /*!< expect reference interrupt enable */

/* CTC interrupt source definitions */
#define CTC_INT_FLAG_CKOK								CTC_STAT_CKOKIF			  /*!< clock trim OK interrupt flag */
#define CTC_INT_FLAG_CKWARN							  CTC_STAT_CKWARNIF			/*!< clock trim warning interrupt flag */
#define CTC_INT_FLAG_ERR								 CTC_STAT_ERRIF			   /*!< error interrupt flag */
#define CTC_INT_FLAG_EREF								CTC_STAT_EREFIF			  /*!< expect reference interrupt flag */
#define CTC_INT_FLAG_CKERR							   CTC_STAT_CKERR			   /*!< clock trim error bit */
#define CTC_INT_FLAG_REFMISS							 CTC_STAT_REFMISS			 /*!< reference sync pulse miss */
#define CTC_INT_FLAG_TRIMERR							 CTC_STAT_TRIMERR			 /*!< trim value error */

/* CTC flag definitions */
#define CTC_FLAG_CKOK									CTC_STAT_CKOKIF			  /*!< clock trim OK flag */
#define CTC_FLAG_CKWARN								  CTC_STAT_CKWARNIF			/*!< clock trim warning flag */
#define CTC_FLAG_ERR									 CTC_STAT_ERRIF			   /*!< error flag */
#define CTC_FLAG_EREF									CTC_STAT_EREFIF			  /*!< expect reference flag */
#define CTC_FLAG_CKERR								   CTC_STAT_CKERR			   /*!< clock trim error bit */
#define CTC_FLAG_REFMISS								 CTC_STAT_REFMISS			 /*!< reference sync pulse miss */
#define CTC_FLAG_TRIMERR								 CTC_STAT_TRIMERR			 /*!< trim value error bit */

/* function declarations */
/* initialization functions */
/* reset ctc clock trim controller */
void ctc_deinit(void);
/* configure reference signal source polarity */
void ctc_refsource_polarity_config(uint32_t polarity);
/* select reference signal source */
void ctc_refsource_signal_select(uint32_t refs);
/* configure reference signal source prescaler */
void ctc_refsource_prescaler_config(uint32_t prescaler);
/* configure clock trim base limit value */
void ctc_clock_limit_value_config(uint8_t limit_value);
/* configure CTC counter reload value */
void ctc_counter_reload_value_config(uint16_t reload_value);
/* enable CTC trim counter */
void ctc_counter_enable(void);
/* disable CTC trim counter */
void ctc_counter_disable(void);

/* function configuration */
/* configure the IRC48M trim value */
void ctc_irc48m_trim_value_config(uint8_t trim_value);
/* generate software reference source sync pulse */
void ctc_software_refsource_pulse_generate(void);
/* configure hardware automatically trim mode */
void ctc_hardware_trim_mode_config(uint32_t hardmode);

/* reading functions */
/* read CTC counter capture value when reference sync pulse occurred */
uint16_t ctc_counter_capture_value_read(void);
/* read CTC trim counter direction when reference sync pulse occurred */
FlagStatus ctc_counter_direction_read(void);
/* read CTC counter reload value */
uint16_t ctc_counter_reload_value_read(void);
/* read the IRC48M trim value */
uint8_t ctc_irc48m_trim_value_read(void);

/* interrupt & flag functions */
/* enable the CTC interrupt */
void ctc_interrupt_enable(uint32_t interrupt);
/* disable the CTC interrupt */
void ctc_interrupt_disable(uint32_t interrupt);
/* get CTC flag */
FlagStatus ctc_flag_get(uint32_t flag);
/* clear CTC flag */
void ctc_flag_clear(uint32_t flag);
/* get CTC interrupt flag */
FlagStatus ctc_interrupt_flag_get(uint32_t interrupt); 
/* clear CTC interrupt flag */
void ctc_interrupt_flag_clear(uint32_t interrupt);

#endif /* GD32F3X0_CTC_H */
