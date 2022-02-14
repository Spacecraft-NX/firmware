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

#include <logger.h>

void null_logger_start()
{

}

void null_logger_device_type(enum DEVICE_TYPE dt)
{

}

void null_logger_glitching_started()
{

}

void null_logger_payload_flash_res_and_cid(uint32_t ret, uint8_t *cid)
{

}

void null_logger_new_config_and_save(glitch_cfg_t *new_cfg, int save_ret)
{

}

void null_logger_2_and_3(glitch_cfg_t *new_config, uint8_t flags, unsigned int datalen, void *data, uint8_t unk)
{

}

void null_logger_end()
{

}

void null_logger_adc(uint32_t value)
{

}

logger null_logger =
{
	null_logger_start,
	null_logger_device_type,
	null_logger_glitching_started,
	null_logger_payload_flash_res_and_cid,
	null_logger_new_config_and_save,
	null_logger_2_and_3,
	null_logger_end,
	null_logger_adc
};