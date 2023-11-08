#include "adc.h"
#include "board.h"
#include "lcd.h"
#include "os.h"

volatile uint16_t joy_adc_values[4] = {0};

void buttons_handler(void* par) {
	// ADC DMA (Joystick)
	if (HAL_ADCEx_InjectedStart(&hadc1) != HAL_OK) {} /*debug message if needed*/
	if (HAL_ADCEx_MultiModeStart_DMA(&hadc1, (volatile void*)joy_adc_values, 4 / 2) != HAL_OK) {
	} /*debug message if needed*/

	while (1) {
		osDelay(4);
		/* TODO: button update function */
	}
}

int controller_tft(int r) {
	tft_prints(0, 1, "Joy L > x: %d y: %d btn: %d", joy_adc_values[0], joy_adc_values[1],
			   HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6));
	tft_prints(0, 2, "Joy R > x: %d y: %d btn: %d", joy_adc_values[2], joy_adc_values[3],
			   HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4));
	return r;
}
