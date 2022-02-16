/*
 * Copyright (c) 2022 HWFLY-NX
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

#ifndef __GLITCH_HEURISTC_H__
#define __GLITCH_HEURISTC_H__

#include <stdint.h>
#include <stdbool.h>
#include <glitch.h>

typedef struct
{
	uint8_t total_count;
	uint8_t no_comms_count;
	uint8_t timeout_count;
	uint8_t block_read_count;

} glitch_heuristic_t;

void heuristic_add_result(glitch_heuristic_t *heuristic, enum GLITCH_RESULT_TYPE result);
void heuristic_advice(glitch_heuristic_t *heuristic, bool *fatal_abort, bool *try_next_offset, int *width_adjust, int *offset_adjust);

#endif
