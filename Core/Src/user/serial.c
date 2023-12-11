#include "serial.h"

#include "board.h"
#include "usart.h"

#include "stm32f4xx_hal_uart.h"

void debug_thread(void* par) {
	uint8_t dat = 0;

	uint32_t last_ticks = 0;
	while (1) {
		if (get_ticks() - last_ticks > 10) {
			HAL_UART_Transmit(&huart1, (uint8_t*)&dat, sizeof(dat), 0xFFFF);
			last_ticks = get_ticks();
			dat++;
		}
		osDelay(4);
	}
}
