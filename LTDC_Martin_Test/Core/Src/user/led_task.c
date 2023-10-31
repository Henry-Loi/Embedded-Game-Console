#include "led_task.h"

#include "gpio.h"

void led_blinky() {
	uint32_t last_ticks = 0;
	uint8_t LED = 0;

	while (1) {
		osDelay(4);
		if (HAL_GetTick() - last_ticks >= 100) {
			if (LED) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, SET);
			} else {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
			}
			LED = (++LED) % 2;
			last_ticks = HAL_GetTick();
		}
	}
}