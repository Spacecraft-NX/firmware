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

#ifndef __MMC_H__
#define __MMC_H__

#include <stdint.h>

uint32_t mmc_initialize(uint8_t *cid);
uint32_t mmc_read(uint32_t offset, uint8_t *block);
uint32_t mmc_write(uint32_t offset, const uint8_t *block);
uint32_t mmc_check_and_if_different_write(uint32_t offset, const uint8_t *buffer, uint32_t len);
uint32_t mmc_check_and_if_header_different_write_all(uint32_t offset, const uint8_t *buffer, uint32_t len);
uint32_t mmc_copy(uint32_t dest, uint32_t source, uint32_t len);
uint32_t mmc_erase(uint32_t offset, uint32_t len);

#endif
