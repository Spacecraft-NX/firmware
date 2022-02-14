/*
 * Copyright (c) 2020 Spacecraft-NX
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

#ifndef __FPGA_H__
#define __FPGA_H__

#include <stdint.h>

void fpga_init();
uint32_t fpga_reset();
void fpga_power_off();

void fpga_select_active_buffer(uint8_t value);
void fpga_reset_device(int do_clock_stuck_glitch);
typedef struct
{
	uint32_t width;
	uint32_t offset;
	uint32_t rng;
} glitch_cfg_t;
void fpga_glitch_device(glitch_cfg_t *cfg);
uint32_t fpga_read_glitch_flags();
uint32_t fpga_read_mmc_flags();

void fpga_do_mmc_command();

void fpga_read_buffer(uint8_t *buffer, uint32_t size);
void fpga_write_buffer(uint8_t *buffer, uint32_t size);

void fpga_enter_cmd_mode();
void fpga_pre_recv();
void fpga_post_recv();
void fpga_post_send();

#endif
