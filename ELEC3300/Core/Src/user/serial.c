#include "serial.h"

#include "board.h"
#include "gpio.h"
#include "icm20602.h"
#include "usart.h"
// #include "stdarg.h"
// #include "strhelper.h"

#include "stm32f4xx_hal_uart.h"


// will freeze the board fml idk why adsfsdsfjklks I give up
/* void uart_send_string(const char* fmt, ...) {
	char buf[255] = {'\0'}, *fp = buf;

	va_list args;
	va_start(args, fmt);
	print(&fp, fmt, args);

	volatile int x = 0;
	for (int i = 0; i < 2; i++) {
		HAL_UART_Transmit_IT(&huart1, (uint8_t*)(&(buf[i])), sizeof(buf[i]));
		x++;
	}
} */

void debug_handler(void* par) {
	// UART_HandleTypeDef* targetUART = (UART_HandleTypeDef*)par;
	// static const char dat[] = "hello world";
	uint8_t dat = 0;
	uint32_t last_ticks = 0;
	int16_t gyro[3] = {0};

	// icm20602_init();
	while (1) {
		// old
		/* 		if (get_ticks() - last_ticks > 10) {
					icm20602_get_gyro_adc(gyro);
					HAL_UART_Transmit(&huart1, (uint8_t*)&dat, sizeof(dat), 0x10);
					for (int i = 0; i < 3; i++) {
						HAL_UART_Transmit(&huart1, (int8_t*)&gyro[i], sizeof(gyro[i]), 0x10);
					}
					dat++;

					last_ticks = get_ticks();
				} */

		osDelay(4);
	}
}
