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

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <config.h>
#include <device.h>

typedef struct 
{
	void (*start)();
	void (*device_type)(enum DEVICE_TYPE dt);
	void (*glitching_started)();
	void (*payload_flash_res_and_cid)(uint32_t ret, uint8_t *cid);
	void (*new_config_and_save)(struct glitch_config *new_cfg, int save_ret);
	void (*_2_and_3)(struct glitch_config *new_config, uint8_t flags, unsigned int datalen, void *data, uint8_t unk);
	void (*end)();
	void (*adc)(uint32_t value);
} logger;

extern logger null_logger;

#endif