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

#include <glitch.h>
#include <fpga.h>
#include <device.h>
#include <adc.h>
#include <leds.h>
#include <string.h>
#include <delay.h>
#include <mmc.h>
#include <config.h>
#include <payload.h>
#include "mmc_defs.h"

typedef struct 
{
	uint8_t *data;
	uint32_t datalen;
	uint8_t cmd;
} mmc_sniff_parser_ctx;

void mmc_sniff_parser_init(mmc_sniff_parser_ctx *ctx, uint8_t *data, int datalen)
{
	ctx->data = data;
	ctx->datalen = datalen;
	ctx->cmd = 0;
}

int mmc_sniff_parser_parse(mmc_sniff_parser_ctx *ctx)
{
	if (ctx->datalen <= 5)
		return 3;

	uint8_t *data = ctx->data;
	char flags = data[0];
	
	int ret = 0;
	if (flags & 0x40)
	{
		ctx->cmd = flags & 0x3F;
		ret = 0;
	}
	else
	{
		if (ctx->datalen <= 0x10)
			return 3;

		if (ctx->cmd == MMC_ALL_SEND_CID || ctx->cmd == MMC_SEND_CSD)
		{
			ctx->cmd = MMC_GO_IDLE_STATE;
			ret = 2;
		}
		else
		{
			ctx->cmd = flags & 0x3F;
			ret = 1;
		}
	}

	if (ret == 2)
	{
		ctx->data += 0x11;
		ctx->datalen -= 0x11;
	}
	else
	{
		ctx->data += 6;
		ctx->datalen -= 6;
	}

	return ret;
}

int read_glitch_result(uint8_t *buf)
{
	if (fpga_read_mmc_flags() & 0x40)
	{
		fpga_select_active_buffer(0);
		fpga_read_buffer(buf, 512);
		int datalen = buf[0x10];

		fpga_select_active_buffer(2);
		fpga_read_buffer(buf, 512);
		return datalen;
	}
	return 0;
}

uint16_t erista_offsets[] = { 825, 830, 835, 840, 845, 850, 855, 860, 865, 870, 875, 880, 885, 890, 895, 900, 905 };
uint16_t mariko_offsets[] = { 820, 825, 830, 835, 840, 845, 850, 855, 860, 865, 870, 875, 880 };

uint32_t rand_seed = 0;
int rand()
{
	rand_seed = 1664525 * rand_seed + 1013904223;
	return rand_seed;
}

int glitch(logger *lgr)
{
	uint32_t start_time = SysTick->VAL;

	config cfg;
	int needs_reflash = config_load(&cfg) == 0xBAD0010B || cfg.reflash;
 
	//qsort();

	enum DEVICE_TYPE device = detect_device_type();
	struct adc_param adc_min_values;
	adc_min_values.min_value = 0;
	adc_min_values.other_value = 0;
	int ret = init_device_specific_adc(device, &adc_min_values);

	uint16_t *offsets;
	unsigned int offsets_count;
	if (device == DEVICE_TYPE_ERISTA)
	{
		offsets = erista_offsets;
		offsets_count = sizeof(erista_offsets) / sizeof(erista_offsets[0]);
	}
	else
	{
		offsets = mariko_offsets;
		offsets_count = sizeof(mariko_offsets) / sizeof(mariko_offsets[0]);
	}

	lgr->start();
	lgr->device_type(device);
	leds_set_pulsing(1);

	unsigned int max_packet_res_1_count;
	if (device == DEVICE_TYPE_ERISTA)
		max_packet_res_1_count = 3;
	else
		max_packet_res_1_count = 5;

	uint16_t adc_goal = 0;
	if (!ret)
	{
		int was_the_device_reset = 0;
		while (1)
		{
			uint16_t adc_read;
			ret = adc_wait_for_min_value(lgr, adc_min_values.other_value, &adc_read);
			if (adc_min_values.other_value <= adc_read)
			{
				adc_goal = adc_min_values.other_value;
				break;
			}
			if (adc_read >= adc_min_values.min_value)
			{
				adc_goal = adc_min_values.min_value;
				ret = 0;
				break;
			}

			if (!was_the_device_reset)
			{
				fpga_reset_device(0);
				was_the_device_reset = 1;
			}
		}
	}

	struct glitch_config glitch_cfg;
	glitch_cfg.rng = 3;
	if (device == DEVICE_TYPE_ERISTA)
	{
		glitch_cfg.width = 35;
		glitch_cfg.offset = 876;
	}
	else
	{
		glitch_cfg.width = 53;
		glitch_cfg.offset = 1210;
	}

	unsigned int saved_idx = 0;

	char packages[128];
	memset(packages, 0, sizeof(packages));
	int glitch_retries = 0;
	if (!ret)
	{
		lgr->glitching_started();
		int offset_idx = 0;
		int package_index = 0;
		while (!ret)
		{
			if (++glitch_retries >= 1200)
			{
				ret = 0xBAD00124;
				break;
			}

			if (glitch_retries >= 400)
			{
				glitch_retries = 0;
				saved_idx = cfg.idx;
				needs_reflash = 1;
			}

			if (adc_wait_eoc_read() < adc_goal)
			{
				ret = adc_wait_for_min_value(lgr, adc_goal, 0);
				if (ret)
					break;
			}

			if (!rand_seed)
				rand_seed = SysTick->VAL - start_time;

			if (needs_reflash)
			{
				needs_reflash = 0;
				if (cfg.reflash)
				{
					cfg.reflash = 0;
					config_save(&cfg);
				}
				uint8_t cid[16];
				ret = flash_payload(cid, device);
				lgr->payload_flash_res_and_cid(ret, cid);
				if (ret != 0x900D0008)
					break;
				ret = 0;
			}

			if (saved_idx < cfg.idx)
			{
				if (glitch_cfg.rng == 3)
				{
					glitch_cfg.width = cfg.timings[saved_idx].width;
					glitch_cfg.offset = cfg.timings[saved_idx].offset;
					glitch_cfg.rng = 0;
					++saved_idx;
				}
				else
					++glitch_cfg.rng;
			}
			else
			{
				if (offsets_count <= ++offset_idx)
					offset_idx = 0;
				glitch_cfg.offset = offsets[offset_idx];
				glitch_cfg.rng = rand() & 3;
			}
			
			if (glitch_cfg.width <= 200)
			{
				if (glitch_cfg.width <= 1)
					glitch_cfg.width = 2;
			}
			else
			{
				glitch_cfg.width = 200;
			}
			fpga_glitch_device(&glitch_cfg);
			int success;
			uint32_t flags;
			while (1)
			{
				flags = fpga_read_mmc_flags();
				success = flags & 2;
				if (flags & 2 || flags & 4)
					break;
				fpga_read_glitch_flags();
			}
			int flag_a = fpga_read_glitch_flags();

			uint8_t data[512];
			int datalen = read_glitch_result(data);
			int packet_res;
			if (datalen >= 5)
				packet_res = 1;
			else
				packet_res = 3;

			mmc_sniff_parser_ctx ctx;
			mmc_sniff_parser_init(&ctx, data, datalen);
			while (1)
			{
				int parser_ret = mmc_sniff_parser_parse(&ctx);
				if (parser_ret == 3) // too less data
					break;
				if (!parser_ret && (ctx.cmd == MMC_READ_SINGLE_BLOCK || ctx.cmd == MMC_GO_IDLE_STATE))
					packet_res = 2;
			}

			lgr->_2_and_3(&glitch_cfg, flags, datalen, data, flag_a);
			packages[package_index] = packet_res;
			package_index = (package_index + 1) & 7;
			int too_short_count = 0;
			int all_package_count = 0;
			int packet_res_1_count = 0;
			int packet_res_2_count = 0;
			for (int j = 0; j < 8; ++j)
			{
				int c = packages[j];
				if (c)
				{
					++all_package_count;
					switch (c)
					{
						case 1:
							++packet_res_1_count;
							break;
						case 2:
							++packet_res_2_count;
							break;
						case 3:
							++too_short_count;
							break;
					}
				}
			}
			if (all_package_count == 8)
			{
				if (too_short_count == 8)
				{
					memset(packages, 0, sizeof(packages));
					ret = 0xBAD00108;
					break;
				}
				else
				{
					if (max_packet_res_1_count > packet_res_1_count)
					{
						if (packet_res_2_count > 4)
						{
							glitch_cfg.width++;
							memset(packages, 0, sizeof(packages));
						}
					}
					else
					{
						glitch_cfg.width--;
						memset(packages, 0, sizeof(packages));
					}
				}
			}
			if (success)
			{
				if (config_add_new(&cfg, &glitch_cfg) == 0x900D0007)
				{
					lgr->new_config_and_save(&glitch_cfg, config_save(&cfg));
				}
				ret = 0x900D0006;
				break;
			}
		}
	}
	lgr->end();
	leds_set_pulsing(0);

	delay_ms(10);
	unsigned int color;
	switch (ret)
	{
		case 0x900D0006:
			color = 0x003F00;	// green
			break;
		case 0xBAD00108:
			color = 0x3F3F3F;	// white
			break;
		case 0xBAD00122:
			color = 0x3F003F;	// magenta
			break;
		default:
			color = 0x3F0000;	// red
			break;
	}
	leds_set_color(color);
	return ret;
}