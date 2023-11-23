#include "serial.h"

#include "board.h"
#include "gpio.h"
#include "usart.h"

#include "stm32f4xx_hal_uart.h"

void debug_handler(void* par) {
	// UART_HandleTypeDef* targetUART = (UART_HandleTypeDef*)par;
	// static const char dat[] = "hello world";
	uint8_t dat = 15;

	uint32_t last_ticks = 0;
	while (1) {
		if (get_ticks() - last_ticks > 1000) {
			HAL_UART_Transmit(&huart1, (uint8_t*)&dat, sizeof(dat), 0xFFFF);
			last_ticks = get_ticks();
		}
		osDelay(4);
	}
}
