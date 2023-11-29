#ifndef TOUCH_BTNS_H
#define TOUCH_BTNS_H

#include "lcd.h"
#include "touch.h"

#include <stdbool.h>

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t x_len;
	uint16_t y_len;
	char* label;
	Color_t border_color;
} TouchBtn_t;

void render_button(TouchBtn_t tar_btn);
void clear_button(TouchBtn_t tar_btn);
bool detect_button_press(TouchBtn_t tar_btn);
#endif