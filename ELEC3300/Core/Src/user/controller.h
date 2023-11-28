#pragma once

#include <stdint.h>

void controller_thread(void* par);
int controller_tft(int r);

typedef enum {
	L_BTN_UP,
	L_BTN_DOWN,
	L_BTN_LEFT,
	L_BTN_RIGHT,
	R_BTN_UP,
	R_BTN_DOWN,
	R_BTN_LEFT,
	R_BTN_RIGHT,
	L_JOY_BTN,
	R_JOY_BTN,
	MAX_NUM_OF_BTNS,
} Buttons_t;

typedef enum {
	L_JOY_X,
	L_JOY_Y,
	R_JOY_X,
	R_JOY_Y,
	MAX_NUM_OF_JOY_AXIS,
} Joystick_t;

typedef struct {
	uint8_t button[MAX_NUM_OF_BTNS];
	uint8_t last_button_state[MAX_NUM_OF_BTNS];
	uint16_t joy_val[MAX_NUM_OF_JOY_AXIS];
	float joystick[MAX_NUM_OF_JOY_AXIS];
	uint16_t joy_offset[MAX_NUM_OF_JOY_AXIS];
	float joy_scale[MAX_NUM_OF_JOY_AXIS];
	uint32_t button_unactive_count;
} Controller;

extern Controller ctrller;