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

#ifndef __GLITCH_H__
#define __GLITCH_H__

#include <stdbool.h>
#include <logger.h>
#include <statuscode.h>
#include <session_info.h>

enum GLITCH_RESULT_TYPE
{
	GLITCH_RESULT_FAIL_NO_EMMC_COMMS = 0,
	GLITCH_RESULT_FAIL_TIMEOUT,
	GLITCH_RESULT_FAILED_MMC,
	GLITCH_RESULT_SUCCESS,
};

enum STATUSCODE glitch(logger *lgr, session_info_t *session_info, bool is_training);

#endif
