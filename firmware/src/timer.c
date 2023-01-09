#include <timer.h>
#include <gd32f3x0.h>

static uint32_t timer_global_start;
static uint32_t timer2_start;
static int timer_counter = 0;
static int timer2_counter_start = 0;

void SysTick_Handler_cnt()
{
	if (SysTick->CTRL & (1u << 16)) // Read and Clear COUNTFLAG.
		timer_counter++;
}

void timer_global_init()
{
	(void)SysTick->CTRL; // Clear COUNTFLAG.
	timer_global_start = (0x1000000 - SysTick->VAL) / 96;
	timer_counter = 0;
}

void timer2_init()
{
	timer2_start = (0x1000000 - SysTick->VAL) / 96;
	timer2_counter_start = timer_counter;
}

uint32_t timer_global_get_us()
{
	uint32_t us = (0x1000000 - SysTick->VAL) / 96;
	us += (uint32_t)timer_counter * 0x2AAAA;
	return us;
}

uint32_t timer2_get_us()
{
	uint32_t us = (0x1000000 - SysTick->VAL) / 96;
	us += (uint32_t)(timer_counter - timer2_counter_start) * 0x2AAAA;
	return us;
}

uint32_t timer_get_global_total()
{
	return timer_global_get_us() - timer_global_start;
}

uint32_t timer2_get_total()
{
	return timer2_get_us() - timer2_start;
}
