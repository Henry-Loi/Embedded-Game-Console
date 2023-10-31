#include "led_task.h"

#include "gpio.h"

uint32_t last_ticks = 0;
uint8_t LED = 0;

void led_blinky() {
	if (HAL_GetTick() - last_ticks >= 500) {
		if (LED) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
		}
		LED = (++LED) % 2;
		last_ticks = HAL_GetTick();
	}
}