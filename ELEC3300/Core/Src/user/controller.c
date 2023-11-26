#include "controller.h"

#include "adc.h"
#include "board.h"
#include "lcd.h"
#include "main.h"
#include "os.h"

controller ctrller = {0};

void button_update(void) {
	/* 	ctrller.button[BTN_UP] = btn_read(Btn_Up);
		ctrller.button[BTN_DOWN] = btn_read(Btn_Down);
		ctrller.button[BTN_LEFT] = btn_read(Btn_Left);
		ctrller.button[BTN_RIGHT] = btn_read(Btn_Right); */
}

void buttons_handler(void* par) {
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
	tft_prints(0, r++, "LBTN UP: %d", btn_read(LBTN_UP));
	tft_prints(0, r++, "LBTN DOWN: %d", btn_read(LBTN_DOWN));
	tft_prints(0, r++, "LBTN LEFT: %d", btn_read(LBTN_LEFT));
	tft_prints(0, r++, "LBTN RIGHT: %d", btn_read(LBTN_RIGHT));

	tft_prints(0, r++, "RBTN UP: %d", btn_read(RBTN_UP));
	tft_prints(0, r++, "RBTN DOWN: %d", btn_read(RBTN_DOWN));
	tft_prints(0, r++, "RBTN LEFT: %d", btn_read(RBTN_LEFT));
	tft_prints(0, r++, "RBTN RIGHT: %d", btn_read(RBTN_RIGHT));
	return r;
}
