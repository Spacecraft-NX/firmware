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

#ifndef __LEDS_H__
#define __LEDS_H__

#include <stdint.h>

enum led_pattern_type
{
	solid,
	pulse,
	blink
};
typedef struct
{
	enum led_pattern_type type;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} led_pattern_t;

void leds_init();

led_pattern_t leds_get_pattern();
void leds_set_pattern(const led_pattern_t *pattern);

// Update led pattern, but wait delay_ms first. Calls to leds_set_pattern cancel delayed changes.
void leds_set_pattern_delayed(const led_pattern_t *pattern, int delay_ms);

// Override led_pattern with supplied one for given duration.
// Led can only be changed immediately with another call to leds_override.
// Other changes are enqueued and applied once duration_ms expires.
void leds_override(uint32_t duration_ms, const led_pattern_t *pattern);

// Turn leds fully off. Requires another call to leds_init() before they can be used again.
void leds_off();

extern led_pattern_t lp_train_prepare;
extern led_pattern_t lp_train_glitching;
extern led_pattern_t lp_train_done;

extern led_pattern_t lp_glitch_prepare;
extern led_pattern_t lp_glitch_glitching;
extern led_pattern_t lp_glitch_done;
extern led_pattern_t lp_flash_payload;

extern led_pattern_t lp_usb;
extern led_pattern_t lp_toolbox;
extern led_pattern_t lp_fw_write;
extern led_pattern_t lp_fw_read;

extern led_pattern_t lp_err_emmc;
extern led_pattern_t lp_err_exhausted;
extern led_pattern_t lp_err_adc;
extern led_pattern_t lp_err_fpga;
extern led_pattern_t lp_err_glitch;
extern led_pattern_t lp_err_firmware;
extern led_pattern_t lp_err_unknown;
extern led_pattern_t lp_config_reset;
extern led_pattern_t lp_off;

#endif