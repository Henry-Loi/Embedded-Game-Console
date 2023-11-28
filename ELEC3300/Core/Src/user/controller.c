#include "controller.h"

#include "adc.h"
#include "board.h"
#include "lcd.h"
#include "main.h"
#include "os.h"

#include "stm32f4xx_hal_adc.h"

#define JOY_SCALE 2000

Controller ctrller = {
	.joy_offset = {[0] = 2044, [1] = 2000, [2] = 2000, [3] = 200},
	.joy_scale = {[0] = -1.0f / JOY_SCALE, [1] = 1.0f / JOY_SCALE, [2] = -1.0f / JOY_SCALE, [3] = 1.0f / JOY_SCALE}};

void button_update(void) {
	ctrller.button[L_BTN_UP] = btn_read(LBTN_UP);
	ctrller.button[L_BTN_DOWN] = btn_read(LBTN_DOWN);
	ctrller.button[L_BTN_LEFT] = btn_read(LBTN_LEFT);
	ctrller.button[L_BTN_RIGHT] = btn_read(LBTN_RIGHT);

	ctrller.button[R_BTN_UP] = btn_read(RBTN_UP);
	ctrller.button[R_BTN_DOWN] = btn_read(RBTN_DOWN);
	ctrller.button[R_BTN_LEFT] = btn_read(RBTN_LEFT);
	ctrller.button[R_BTN_RIGHT] = btn_read(RBTN_RIGHT);

	ctrller.button[L_JOY_BTN] = btn_read(Joy_L_Btn);
	ctrller.button[R_JOY_BTN] = btn_read(Joy_L_Btn);

	for (int i = 0; i < MAX_NUM_OF_JOY_AXIS; i++) {
		ctrller.joystick[i] = ((float)ctrller.joy_val[i] - ctrller.joy_offset[i]) * ctrller.joy_scale[i];
	}

	for (int i = 0; i < MAX_NUM_OF_BTNS; i++) {
		if (ctrller.button[i] != ctrller.last_button_state[i]) {
			ctrller.button_unactive_count = get_ticks();
		}
		ctrller.last_button_state[i] = ctrller.button[i];
	}
}

void controller_thread(void* par) {
	// ADC DMA (Joystick)
	if (HAL_ADC_Start(&hadc1) != HAL_OK) {}										/*debug message if needed*/
	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ctrller.joy_val, 4) != HAL_OK) {} /*debug message if needed*/

	while (1) {
		osDelay(4);
		button_update();
	}
}

int controller_tft(int r) {
	tft_prints(0, r++, "Joy L > x: %f y: %f btn: %d", (double)ctrller.joystick[L_JOY_X],
			   (double)ctrller.joystick[L_JOY_Y], HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4));
	tft_prints(0, r++, "Joy R > x: %f y: %f btn: %d", (double)ctrller.joystick[R_JOY_X],
			   (double)ctrller.joystick[R_JOY_Y], HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5));
	tft_prints(0, r++, "LEFT BTNS: %d %d %d %d", ctrller.button[L_BTN_UP], ctrller.button[L_BTN_DOWN],
			   ctrller.button[L_BTN_LEFT], ctrller.button[L_BTN_RIGHT]);
	tft_prints(0, r++, "RIGHT BTNS: %d %d %d %d", ctrller.button[R_BTN_UP], ctrller.button[R_BTN_DOWN],
			   ctrller.button[R_BTN_LEFT], ctrller.button[R_BTN_RIGHT]);
	return r;
}
