#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

void timer_global_init();
void timer2_init();
uint32_t timer_global_get_us();
uint32_t timer2_get_us();
uint32_t timer_get_global_total();
uint32_t timer2_get_total();

#endif