#include "touch_btns.h"

void render_button(TouchBtn_t tar_btn) {
	tft_draw_line(tar_btn.x, tar_btn.y, tar_btn.x, tar_btn.y + tar_btn.y_len, tar_btn.border_color);
	tft_draw_line(tar_btn.x, tar_btn.y, tar_btn.x + tar_btn.x_len, tar_btn.y, tar_btn.border_color);
	tft_draw_line(tar_btn.x + tar_btn.x_len, tar_btn.y, tar_btn.x + tar_btn.x_len, tar_btn.y + tar_btn.y_len,
				  tar_btn.border_color);
	tft_draw_line(tar_btn.x, tar_btn.y + tar_btn.y_len, tar_btn.x + tar_btn.x_len, tar_btn.y + tar_btn.y_len,
				  tar_btn.border_color);
	// tft_prints((tar_btn.x + tar_btn.x+tar_btn.x_len) / 2 / LCD_MAX_CHAR_WIDTH, (tar_btn.y + tar_btn.y+tar_btn.y_len)
	// / 2 / LCD_MAX_CHAR_HEIGHT,
	//    "%s", tar_btn.label);
}

void clear_button(TouchBtn_t tar_btn) {
	tft_draw_line(tar_btn.x, tar_btn.y, tar_btn.x, tar_btn.y + tar_btn.y_len, WHITE);
	tft_draw_line(tar_btn.x, tar_btn.y, tar_btn.x + tar_btn.x_len, tar_btn.y, WHITE);
	tft_draw_line(tar_btn.x + tar_btn.x_len, tar_btn.y, tar_btn.x + tar_btn.x_len, tar_btn.y + tar_btn.y_len, WHITE);
	tft_draw_line(tar_btn.x, tar_btn.y + tar_btn.y_len, tar_btn.x + tar_btn.x_len, tar_btn.y + tar_btn.y_len, WHITE);
}

bool detect_button_press(TouchBtn_t tar_btn) {
	if (touch_feedback.pressed_state) {
		for (int i = 0; i < touch_feedback.num; i++) {
			if (touch_feedback.point[i].x > tar_btn.x && touch_feedback.point[i].x < tar_btn.x + tar_btn.x_len
				&& touch_feedback.point[i].y > tar_btn.y && touch_feedback.point[i].y < tar_btn.y + tar_btn.y_len) {
				return true;
			}
		}
	}

	return false;
}