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

#include <stdarg.h>
#include <string.h>
#include <stdint.h>

static void printnum(char **p_out_buf, uint32_t value, int base, char fill, int fcnt)
{
	char buf[65];
	static const char digits[] = "0123456789ABCDEFghijklmnopqrstuvwxyz";
	char *p;
	int c = fcnt;

	if (base > 36)
		return;

	p = buf + 64;
	*p = 0;
	do
	{
		c--;
		*--p = digits[value % base];
		value /= base;
	} while (value);

	if (fill != 0)
	{
		while (c > 0)
		{
			*--p = fill;
			c--;
		}
	}

	for (; *p; p++)
		*((*p_out_buf)++) = *p;
}

int vsprintf(char *out_buf, const char *fmt, va_list ap)
{
	int fill, fcnt;

	char *p_out_buf = out_buf;

	while(*fmt)
	{
		if(*fmt == '%')
		{
			fmt++;
			fill = 0;
			fcnt = 0;
			if ((*fmt >= '0' && *fmt <= '9') || *fmt == ' ')
			{
				fcnt = *fmt;
				fmt++;
				if (*fmt >= '0' && *fmt <= '9')
				{
					fill = fcnt;
					fcnt = *fmt - '0';
					fmt++;
				}
				else
				{
					fill = ' ';
					fcnt -= '0';
				}
			}
			switch(*fmt)
			{
			case 'c':
				*p_out_buf++ = va_arg(ap, uint32_t);
				break;
			case 's':
			{
				char *s = va_arg(ap, char *);
				for (; *s; s++)
					*p_out_buf++ = *s;
				break;
			}
			case 'd':
				printnum(&p_out_buf, va_arg(ap, uint32_t), 10, fill, fcnt);
				break;
			case 'p':
			case 'P':
			case 'x':
			case 'X':
				printnum(&p_out_buf, va_arg(ap, uint32_t), 16, fill, fcnt);
				break;
			case '%':
				*p_out_buf++ = '%';
				break;
			case '\0':
				goto out;
			default:
				*p_out_buf++ = '%';
				*p_out_buf++ = *fmt;
				break;
			}
		}
		else
			*p_out_buf++ = *fmt;
		fmt++;
	}

out:
	*p_out_buf = '\0';

	return p_out_buf - out_buf;
}

int sprintf(char *out_buf, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	int ret = vsprintf(out_buf, fmt, ap);

	va_end(ap);

	return ret;
}