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

void null_logger_glitch_result(glitch_cfg_t *new_cfg, uint8_t glitch_res, uint8_t mmc_flags, unsigned int datalen, uint8_t *data, uint8_t glitch_flags)
{

}

void null_logger_end()
{

}

void null_logger_adc(uint32_t value)
{

}

void null_logger_stats(uint32_t attempt, uint16_t offset, uint8_t width, uint8_t subcycle, uint8_t needs_reflash)
{

}


logger null_logger =
{
	null_logger_start,
	null_logger_device_type,
	null_logger_glitching_started,
	null_logger_payload_flash_res_and_cid,
	null_logger_new_config_and_save,
	null_logger_glitch_result,
	null_logger_end,
	null_logger_adc,
	null_logger_stats
};