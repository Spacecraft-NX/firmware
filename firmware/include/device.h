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

#ifndef __DEVICE_H__
#define __DEVICE_H__

enum DEVICE_TYPE
{
	DEVICE_TYPE_UNKNOWN = 0,
	DEVICE_TYPE_ERISTA,
	DEVICE_TYPE_MARIKO,
	DEVICE_TYPE_LITE
};

enum DEVICE_TYPE detect_device_type();

#endif