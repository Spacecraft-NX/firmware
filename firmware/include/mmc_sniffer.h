/*
 * Copyright (c) 2020 Spacecraft-NX
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

#ifndef __MMC_SNIFFER_H__
#define __MMC_SNIFFER_H__

#include <stdint.h>

enum MMC_SNIFFER_PACKET_TYPE
{
	MMC_SNIFF_PKT_TYPE_INVALID = 0,
	MMC_SNIFF_PKT_TYPE_COMMAND, // 48-bit command from host to device
	MMC_SNIFF_PKT_TYPE_RESPONSE48, // 48-bit response from device
	MMC_SNIFF_PKT_TYPE_RESPONSE136, // 136-bit response from device
};

typedef struct
{
	uint8_t *data;
	uint32_t datalen;
	uint8_t cmd;
} mmc_sniff_parser_ctx;

void mmc_sniff_parser_init(mmc_sniff_parser_ctx *ctx, uint8_t *data, int datalen);
enum MMC_SNIFFER_PACKET_TYPE mmc_sniff_parser_parse(mmc_sniff_parser_ctx *ctx);

#endif