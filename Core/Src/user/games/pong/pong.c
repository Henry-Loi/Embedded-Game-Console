#include "pong.h"

#include "board.h"
#include "controller.h"
#include "delay.h"
#include "lcd.h"
#include "main.h"
#include "tft.h"

#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH  LCD_WIDTH	 // window height
#define SCREEN_HEIGHT LCD_HEIGHT // window width


typedef struct ball_s {
	int x, y; /* position on the screen */
	int last_x, last_y;
	int w, h;	// ball width and height
	int dx, dy; /* movement vector */
} ball_t;

typedef struct paddle {
	int x, y;
	int last_x, last_y;
	int w, h;
} paddle_t;

// Program globals
static ball_t ball;
static paddle_t paddle[2];
int score[] = {0, 0};
int width, height; // used if fullscreen


// inisilise starting position and sizes of game elemements
static void init_game() {
	ball.x = SCREEN_WIDTH / 2;
	ball.y = rand() % SCREEN_HEIGHT;
	ball.w = 10;
	ball.h = 10;
	ball.dy = 1;
	ball.dx = 1;

	paddle[0].x = 20;
	paddle[0].y = SCREEN_HEIGHT / 2 - 50;
	paddle[0].w = 10;
	paddle[0].h = 50;

	paddle[1].x = SCREEN_WIDTH - 20 - 10;
	paddle[1].y = SCREEN_HEIGHT / 2 - 50;
	paddle[1].w = 10;
	paddle[1].h = 50;
}

int check_score() {
	int i;

	// loop through player scores
	for (i = 0; i < 2; i++) {
		// check if score is @ the score win limit
		if (score[i] == WINING_SCORE) {
			// reset scores
			score[0] = 0;
			score[1] = 0;

			// return 1 if player 1 score @ limit
			if (i == 0) {
				return 1;

				// return 2 if player 2 score is @ limit
			} else {
				return 2;
			}
		}
	}

	// return 0 if no one has reached a score of 10 yet
	return 0;
}

// if return value is 1 collision occured. if return is 0, no collision.
int check_collision(ball_t a, paddle_t b) {
	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	left_b = b.x;
	right_b = b.x + b.w;
	top_b = b.y;
	bottom_b = b.y + b.h;


	if (left_a > right_b) {
		return 0;
	}

	if (right_a < left_b) {
		return 0;
	}

	if (top_a > bottom_b) {
		return 0;
	}

	if (bottom_a < top_b) {
		return 0;
	}

	return 1;
}

/* This routine moves each ball by its motion vector. */
static void move_ball() {
	ball.last_x = ball.x;
	ball.last_y = ball.y;

	/* Move the ball by its motion vector. */
	ball.x += ball.dx;
	ball.y += ball.dy;

	/* Turn the ball around if it hits the edge of the screen. */
	if (ball.x < 0) {
		score[1] += 1;
		tft_prints(LCD_MAX_CHAR_WIDTH / 2 - 6, 1, "%d          %d", score[0], score[1]);
		tft_clear(BLACK);
		init_game();
	}

	if (ball.x > SCREEN_WIDTH - 10) {
		score[0] += 1;
		tft_prints(LCD_MAX_CHAR_WIDTH / 2 - 6, 1, "%d          %d", score[0], score[1]);
		tft_clear(BLACK);
		init_game();
	}

	if (ball.y < 0 || ball.y > SCREEN_HEIGHT - 10) {
		ball.dy = -ball.dy;
	}

	// check for collision with the paddle
	int i;

	for (i = 0; i < 2; i++) {
		int c = check_collision(ball, paddle[i]);

		// collision detected
		if (c == 1) {
			// ball moving left
			if (ball.dx < 0) {
				ball.dx -= 1;

				// ball moving right
			} else {
				ball.dx += 1;
			}

			// change ball direction
			ball.dx = -ball.dx;

			// change ball angle based on where on the paddle it hit
			int hit_pos = (paddle[i].y + paddle[i].h) - ball.y;

			if (hit_pos >= 0 && hit_pos < 7) {
				ball.dy = 4;
			}

			if (hit_pos >= 7 && hit_pos < 14) {
				ball.dy = 3;
			}

			if (hit_pos >= 14 && hit_pos < 21) {
				ball.dy = 2;
			}

			if (hit_pos >= 21 && hit_pos < 28) {
				ball.dy = 1;
			}

			if (hit_pos >= 28 && hit_pos < 32) {
				ball.dy = 0;
			}

			if (hit_pos >= 32 && hit_pos < 39) {
				ball.dy = -1;
			}

			if (hit_pos >= 39 && hit_pos < 46) {
				ball.dy = -2;
			}

			if (hit_pos >= 46 && hit_pos < 53) {
				ball.dy = -3;
			}

			if (hit_pos >= 53 && hit_pos <= 60) {
				ball.dy = -4;
			}

			// ball moving right
			if (ball.dx > 0) {
				// teleport ball to avoid mutli collision glitch
				if (ball.x < 30) {
					ball.x = 30;
				}

				// ball moving left
			} else {
				// teleport ball to avoid mutli collision glitch
				if (ball.x > LCD_WIDTH - 30) {
					ball.x = LCD_WIDTH;
				}
			}
		}
	}
}

static void move_paddle_ai() {
	paddle[0].last_y = paddle[0].y;
	paddle[0].last_x = paddle[0].x;

	int center = paddle[0].y + 25;
	int screen_center = SCREEN_HEIGHT / 2 - 25;
	int ball_speed = ball.dy;

	if (ball_speed < 0) {
		ball_speed = -ball_speed;
	}

	// ball moving right
	if (ball.dx > 0) {
		// return to center position
		if (center < screen_center) {
			paddle[0].y += ball_speed;

		} else {
			paddle[0].y -= ball_speed;
		}

		// ball moving left
	} else {
		// ball moving down
		if (ball.dy > 0) {
			if (ball.y > center) {
				paddle[0].y += ball_speed;

			} else {
				paddle[0].y -= ball_speed;
			}
		}

		// ball moving up
		if (ball.dy < 0) {
			if (ball.y < center) {
				paddle[0].y -= ball_speed;

			} else {
				paddle[0].y += ball_speed;
			}
		}

		// ball moving stright across
		if (ball.dy == 0) {
			if (ball.y < center) {
				paddle[0].y -= 5;

			} else {
				paddle[0].y += 5;
			}
		}
	}
}

static void move_paddle(int d) {
	// if the down arrow is pressed move paddle down
	if (d == 0) {
		paddle[1].last_y = paddle[1].y;
		paddle[1].last_x = paddle[1].x;
		if (paddle[1].y >= SCREEN_HEIGHT - paddle[1].h) {
			paddle[1].y = SCREEN_HEIGHT - paddle[1].h;

		} else {
			paddle[1].y += 5;
		}
	}

	// if the up arrow is pressed move paddle up
	if (d == 1) {
		paddle[1].last_y = paddle[1].y;
		paddle[1].last_x = paddle[1].x;
		if (paddle[1].y <= 0) {
			paddle[1].y = 0;

		} else {
			paddle[1].y -= 5;
		}
	}
}

// for multiplayer only
static void move_paddle2(int d) {
	// if the down arrow is pressed move paddle down
	if (d == 0) {
		paddle[0].last_y = paddle[0].y;
		paddle[0].last_x = paddle[0].x;
		if (paddle[0].y >= SCREEN_HEIGHT - paddle[0].h) {
			paddle[0].y = SCREEN_HEIGHT - paddle[0].h;

		} else {
			paddle[0].y += 5;
		}
	}

	// if the up arrow is pressed move paddle up
	if (d == 1) {
		paddle[0].last_y = paddle[0].y;
		paddle[0].last_x = paddle[0].x;
		if (paddle[0].y <= 0) {
			paddle[0].y = 0;

		} else {
			paddle[0].y -= 5;
		}
	}
}

static void draw_game_over(int p) {
	tft_prints(LCD_MAX_CHAR_WIDTH / 2 - 20, LCD_MAX_CHAR_HEIGHT / 2 - 1,
			   "Game Over (Click Right Right Key to restart)");
}

static void draw_menu() {
	tft_prints(LCD_MAX_CHAR_WIDTH / 2 - 2, LCD_MAX_CHAR_HEIGHT / 2 - 1, "Menu");
	tft_prints(LCD_MAX_CHAR_WIDTH / 2 - 25, LCD_MAX_CHAR_HEIGHT / 2 + 1,
			   "Single Player (Left Key) Multi Player (Right Key)");
}


static void draw_net() { tft_draw_line(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE); }

static void draw_ball() {
	tft_fill_rect(ball.last_x, ball.last_y, ball.last_x + ball.w, ball.last_y + ball.h, BLACK);
	tft_fill_rect(ball.x, ball.y, ball.x + ball.w, ball.y + ball.h, YELLOW);
}

static void draw_paddle() {
	for (int i = 0; i < 2; i++) {
		tft_fill_rect(paddle[i].last_x, paddle[i].last_y, paddle[i].last_x + paddle[i].w,
					  paddle[i].last_y + paddle[i].h, BLACK);
	}
	tft_fill_rect(paddle[0].x, paddle[0].y, paddle[0].x + paddle[0].w, paddle[0].y + paddle[0].h, RED);
	tft_fill_rect(paddle[1].x, paddle[1].y, paddle[1].x + paddle[1].w, paddle[1].y + paddle[1].h, BLUE);
}

static void draw_player_0_score() { tft_prints(12, 1, "%d", score[0]); }

static void draw_player_1_score() { tft_prints(20, 1, "%d", score[1]); }

void pong_thread(void* par) {
	// tft_clear(BLACK);

	uint8_t is_single_player = 1;
	int state = 0;
	int r = 0;
	uint32_t last_game_tick = get_ticks();
	srand(time(NULL));

	// Initialize the ball position data.
	init_game();
	uint8_t in_game_once_flag = 0;


	// render loop
	while (1) {
		osDelay(4);

		if (curr_page != PONG_PAGE) {
			continue;
		}

		tft_set_text_color(WHITE);
		led_on(LED8);

		// HAL_Delay(100);

		if (gpio_read(RBTN_UP) || ctrller.joystick[R_JOY_Y] < 0) {
			move_paddle(0);
		}

		if (gpio_read(RBTN_DOWN) || ctrller.joystick[R_JOY_Y] > 0) {
			move_paddle(1);
		}

		if (!is_single_player) {
			if (gpio_read(LBTN_UP) || ctrller.joystick[L_JOY_Y] < -0.1f) {
				move_paddle2(0);
			}

			if (gpio_read(LBTN_DOWN) || ctrller.joystick[L_JOY_Y] > 0.1f) {
				move_paddle2(1);
			}
		}


		// display main menu
		if (state == 0) {
			if (in_game_once_flag == 0) {
				tft_clear(BLACK);
				in_game_once_flag = 1;
			}
			if (!gpio_read(RBTN_RIGHT) || !gpio_read(RBTN_LEFT)) {
				if (!gpio_read(RBTN_RIGHT)) {
					is_single_player = 0;
				} else {
					is_single_player = 1;
				}
				state = 1;
				in_game_once_flag = 0;
				tft_prints(LCD_MAX_CHAR_WIDTH / 2 - 6, 1, "%d          %d", score[0], score[1]);
			} else {
				// draw menu
				draw_menu();
			}

			// display gameover
		} else if (state == 2) {
			if (!gpio_read(RBTN_RIGHT) || !gpio_read(RBTN_LEFT)) {
				if (!gpio_read(RBTN_RIGHT)) {
					is_single_player = 0;
				} else {
					is_single_player = 1;
				}
				state = 0;
				tft_clear(BLACK);
				in_game_once_flag = 0;
				// delay for a little bit so the space bar press dosnt get triggered twice
				// while the main menu is showing
				delay(500);
			}

			if (r == 1) {
				// if player 1 is AI if player 1 was human display the return value of r not 3
				draw_game_over(3);

			} else {
				// display gameover
				draw_game_over(r);
			}

			// display the game
		} else if (state == 1) {
			if (in_game_once_flag == 0) {
				tft_clear(BLACK);
				in_game_once_flag = 1;
			}
			// check score
			r = check_score();

			// if either player wins, change to game over state
			if (r == 1) {
				state = 2;

			} else if (r == 2) {
				state = 2;
			}

			// paddle ai movement
			if (is_single_player) {
				move_paddle_ai();
			}

			//* Move the balls for the next frame.
			move_ball();

			// draw net
			draw_net();

			// draw paddles
			draw_paddle();

			//* Put the ball on the screen.
			draw_ball();
		}

		tft_update();
	}
}
