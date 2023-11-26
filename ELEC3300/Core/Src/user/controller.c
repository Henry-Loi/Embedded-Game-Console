#include "controller.h"

#include "adc.h"
#include "board.h"
#include "lcd.h"
#include "main.h"
#include "os.h"

Controller ctrller = {0};

void button_update(void) {
	ctrller.button[L_BTN_UP] = btn_read(LBTN_UP);
	ctrller.button[L_BTN_DOWN] = btn_read(LBTN_DOWN);
	ctrller.button[L_BTN_LEFT] = btn_read(LBTN_LEFT);
	ctrller.button[L_BTN_RIGHT] = btn_read(LBTN_RIGHT);

	ctrller.button[R_BTN_UP] = btn_read(RBTN_UP);
	ctrller.button[R_BTN_DOWN] = btn_read(RBTN_DOWN);
	ctrller.button[R_BTN_LEFT] = btn_read(RBTN_LEFT);
	ctrller.button[R_BTN_RIGHT] = btn_read(RBTN_RIGHT);
}

void controller_thread(void* par) {
	// ADC DMA (Joystick)
	if (HAL_ADCEx_InjectedStart(&hadc1) != HAL_OK) {} /*debug message if needed*/
	if (HAL_ADCEx_MultiModeStart_DMA(&hadc1, (volatile void*)ctrller.joystick, 4 / 2) != HAL_OK) {
	} /*debug message if needed*/

	while (1) {
		osDelay(4);
		button_update();
	}
}

int controller_tft(int r) {
	/* 	tft_prints(0, r++, "Joy L > x: %d y: %d btn: %d", ctrller.joystick[JOY_L_X], ctrller.joystick[JOY_L_Y],
				   HAL_btn_readPin(GPIOE, GPIO_PIN_6));
		tft_prints(0, r++, "Joy R > x: %d y: %d btn: %d", ctrller.joystick[JOY_L_X], ctrller.joystick[JOY_R_X],
				   HAL_btn_readPin(GPIOE, GPIO_PIN_4));
		tft_prints(0, r++, "Btn > up: %d down: %d left: %d right: %d", ctrller.button[BTN_UP], ctrller.button[BTN_DOWN],
				   ctrller.button[BTN_LEFT], ctrller.button[BTN_RIGHT]); */
	tft_prints(0, r++, "LEFT BTNS: %d %d %d %d", ctrller.button[L_BTN_UP], ctrller.button[L_BTN_DOWN],
			   ctrller.button[L_BTN_LEFT], ctrller.button[L_BTN_RIGHT]);
	tft_prints(0, r++, "RIGHT BTNS: %d %d %d %d", ctrller.button[R_BTN_UP], ctrller.button[R_BTN_DOWN],
			   ctrller.button[R_BTN_LEFT], ctrller.button[R_BTN_RIGHT]);
	return r;
}
