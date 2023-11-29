/*
 * tetris - a small ncursess tetris game
 * Copyright (C) 2016-2019 Alessandro Righi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "screen.h"

#include "controller.h"
#include "lcd.h"
#include "score.h"
#include "tetris_game.h"
#include "touch_btns.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/* filled block character */
// static const chtype BLOCK = ' ' | A_REVERSE;

// static WINDOW *game_win, *score_win, *title_win;


#define BLOCK_LEN	 30
#define BORDER_WIDTH 10
#define TOP_PADDING	 10

int screen[MAX_Y][MAX_X] = {{0}};

static void print_matrix(void) {
	for (int y = 1; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			Color_t curr_color = WHITE;
			switch (screen[y][x]) {
				case 1: curr_color = RED; break;
				case 2: curr_color = BLUE; break;
				case 3: curr_color = GREEN; break;
				case 4: curr_color = YELLOW; break;
				case 5: curr_color = BROWN; break;
				case 6: curr_color = PINK; break;
				case 7: curr_color = GBLUE; break;
			}

			tft_fill_rect(x * BLOCK_LEN + 1 + LEFT_PADDING, y * BLOCK_LEN + 1 + TOP_PADDING,
						  (x + 1) * BLOCK_LEN - 1 + LEFT_PADDING, (y + 1) * BLOCK_LEN - 1 + TOP_PADDING, curr_color);
			tft_fill_rect(x * BLOCK_LEN + LEFT_PADDING, y * BLOCK_LEN + TOP_PADDING, (x + 1) * BLOCK_LEN + LEFT_PADDING,
						  y * BLOCK_LEN + 1 + TOP_PADDING, BLACK);
			tft_fill_rect(x * BLOCK_LEN + LEFT_PADDING, y * BLOCK_LEN + TOP_PADDING, x * BLOCK_LEN + LEFT_PADDING,
						  (y + 1) * BLOCK_LEN + 1 + TOP_PADDING, BLACK);
			tft_fill_rect((x + 1) * BLOCK_LEN + LEFT_PADDING, y * BLOCK_LEN + TOP_PADDING,
						  (x + 1) * BLOCK_LEN + 1 + LEFT_PADDING, (y + 1) * BLOCK_LEN + 1 + TOP_PADDING, BLACK);
			tft_fill_rect(x * BLOCK_LEN + LEFT_PADDING, (y + 1) * BLOCK_LEN + TOP_PADDING,
						  (x + 1) * BLOCK_LEN + 1 + LEFT_PADDING, (y + 1) * BLOCK_LEN + 1 + TOP_PADDING, BLACK);
		}
	}
}

static void print_score(void) {
	char next[4][4];

	tft_prints(45, 0, "LEVEL: %d                      ", level);
	tft_prints(45, 1, "SCORE: %d                      ", tetris_score);

	get_next_piece(next);
}

static void refresh_screen(void) {
	print_score();
	print_matrix();
}

void prompt_new_game(void) {
	refresh_screen();
	// wclear(score_win);
	// wprintw(score_win, "Sorry, you lost :( score %d\n", score);
	tft_prints(45, 3, "GAME OVER");
	tft_prints(45, 4, "FINAL SCORE: %d", tetris_score);
	tft_prints(45, 5, "PRESS ANY KEY TO");
	tft_prints(45, 6, "CONTINUE");

	for (int i = 0; i < MAX_NUM_OF_BTNS; i++) {
		if (ctrller.button[i]) {
			gameover = false;
			break;
		}
	}
	// wprintw(score_win, "Start a new game ? (y/n)");
	// wrefresh(score_win);

	// while ((ch = getch()) != 'y') {
	// if (ch == 'n')
	// exit(EXIT_SUCCESS);
	// }

	if (!gameover)
		start_new_game();
}


TouchBtn_t t_btn = {.x = 100, .y = 100, .x_len = 250, .y_len = 250, .label = "test", .border_color = RED};
enum { V_L_BTN, V_R_BTN, V_DOWN_BTN, V_BOT_BTN, V_ROT_BTN, V_ROT_R_BTN, V_HOME_BTN, MAX_NUM_OF_V_BTNS };

TouchBtn_t tetris_screen_btns[MAX_NUM_OF_V_BTNS] = {
	{.x = 20, .y = 400, .x_len = 150, .y_len = 150, .label = "LEFT", .border_color = RED},
	{.x = 180, .y = 400, .x_len = 150, .y_len = 150, .label = "RIGHT", .border_color = RED},
	{.x = 100, .y = 230, .x_len = 150, .y_len = 150, .label = "DOWN", .border_color = RED},
	{.x = LCD_WIDTH - 100 - 150, .y = 230, .x_len = 150, .y_len = 150, .label = "BOT", .border_color = RED},
	{.x = LCD_WIDTH - 20 - 150, .y = 400, .x_len = 150, .y_len = 150, .label = "ROT", .border_color = RED},
	{.x = LCD_WIDTH - 180 - 150, .y = 400, .x_len = 150, .y_len = 150, .label = "R ROT", .border_color = RED},
	{.x = 100, .y = 100, .x_len = 150, .y_len = 150, .label = "HOME", .border_color = RED},
};

bool tetris_do_once = true;
void tetris_thread(void* par) {
	// srand((unsigned)time(NULL));
	// init_score();
	// start_new_game();
	// main_loop();
	uint32_t last_ticks = 0;
	while (1) {
		osDelay(4);

		tft_update();
		if (gameover) {
			if (tetris_do_once) {
				tetris_do_once = false;
				gameover = false;
				start_new_game();
			} else {
				prompt_new_game();
			}
			continue;
		}
		// tft_set_text_color(BLACK);
		if (get_ticks() - last_ticks > 500) {
			last_ticks = get_ticks();
			move_down();
		}

		// tft_prints(30, 18, "test: %d", detect_button_press(t_btn));
		for (int i = 0; i < 6; i++) {
			render_button(tetris_screen_btns[i]);
		}
		if (detect_button_press(tetris_screen_btns[V_L_BTN])) {
			move_left();
		}
		if (detect_button_press(tetris_screen_btns[V_R_BTN])) {
			move_right();
		}
		if (detect_button_press(tetris_screen_btns[V_DOWN_BTN])) {
			move_down();
		}
		if (detect_button_press(tetris_screen_btns[V_BOT_BTN])) {
			move_bottom();
		}
		if (detect_button_press(tetris_screen_btns[V_ROT_BTN])) {
			rotate();
		}
		if (detect_button_press(tetris_screen_btns[V_ROT_R_BTN])) {
			rotate2();
		}

		/* 		for (int y = 0; y < MAX_Y; y++) {
					for (int x = 0; x < MAX_X; x++) {
						tft_prints(x + 40, y, "%d", screen[y][x]);
					}
				} */
		if (ctrller.button[L_BTN_UP] || ctrller.joystick[R_JOY_X] < -0.5) {
			rotate();
		} else if (ctrller.joystick[R_JOY_X] > 0.5) {
			rotate2();
		} else if (ctrller.button[L_BTN_LEFT] || ctrller.joystick[L_JOY_X] < -0.5) {
			move_left();
		} else if (ctrller.button[L_BTN_RIGHT] || ctrller.joystick[L_JOY_X] > 0.5) {
			move_right();
		} else if (ctrller.button[L_BTN_DOWN] || ctrller.joystick[L_JOY_Y] < -0.5) {
			move_down();
		}
		if (ctrller.button[R_BTN_DOWN] || ctrller.button[L_JOY_BTN] || ctrller.button[R_JOY_BTN]) {
			move_bottom();
		}

		refresh_screen();
		// draw_windows();
	}
}
