#include "delay.h"

static uint32_t g_fac_us = 180;

void delay(uint32_t ms) { HAL_Delay(ms); }

void delay_us(uint32_t us) {
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD;
	ticks = us * g_fac_us;

	told = SysTick->VAL;
	while (1) {
		tnow = SysTick->VAL;
		if (tnow != told) {
			if (tnow < told) {
				tcnt += told - tnow;
			} else {
				tcnt += reload - tnow + told;
			}
			told = tnow;
			if (tcnt >= ticks) {
				break;
			}
		}
	}
}