#include "tetris.h"

#include "lcd.h"

#define BLOCK_LEN	 30
#define BORDER_WIDTH 10
#define TOP_PADDING	 10

#define MAX_X 22
#define MAX_Y 19

uint8_t tetris_game_board[22][19];


typedef enum {
	STRAIGHT_PIECE = 0,
	SQUARE_PIECE,
	T_PIECE,
	L_PIECE,
	REVERSE_L_PIECE,
	Z_PIECE,
	S_PIECE,
} Piece_t;

void draw_game_window(void) {
	tft_fill_rect(0, 0, BORDER_WIDTH, 600, RED);
	tft_fill_rect(0, 0, 670, BORDER_WIDTH, RED);
	tft_fill_rect(670, 0, 670 + BORDER_WIDTH, 600, RED);
	tft_fill_rect(0, 600 - BORDER_WIDTH, 670 + BORDER_WIDTH, 600, RED);
}

void draw_block(uint16_t x, uint16_t y, Color_t color) {
	tetris_game_board[x][y] = color == WHITE ? 0 : 1;
	x = x * BLOCK_LEN + BORDER_WIDTH;
	y = y * BLOCK_LEN + BORDER_WIDTH + TOP_PADDING;
	tft_fill_rect(x + 2, y + 2, x + BLOCK_LEN - 2, y + BLOCK_LEN - 2, color);

	tft_fill_rect(x, y, x + BLOCK_LEN, y + 2, BLACK);
	tft_fill_rect(x, y, x + 2, y + BLOCK_LEN, BLACK);
	tft_fill_rect(x + 2, y + BLOCK_LEN - 2, x + BLOCK_LEN, y + BLOCK_LEN, BLACK);
	tft_fill_rect(x + BLOCK_LEN - 2, y + 2, x + BLOCK_LEN, y + BLOCK_LEN, BLACK);
}

void draw_piece(uint16_t x, uint16_t y, uint16_t piece, Color_t color) {
	switch (piece) {
		case 0:
			for (int i = 0; i < 4; i++) {
				draw_block(x + i, y, color);
			}

			break;
		case 1:
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					draw_block(x + i, y + j, color);
				}
			}
			break;
		case 2:
			draw_block(x + 1, y, color);
			draw_block(x, y + 1, color);
			draw_block(x + 1, y + 1, color);
			draw_block(x + 2, y + 1, color);

			break;
		case 3:
			draw_block(x + 2, y + 1, color);
			draw_block(x, y, color);
			draw_block(x + 1, y, color);
			draw_block(x + 2, y, color);
			break;

		case 4:
			draw_block(x, y + 1, color);
			draw_block(x, y, color);
			draw_block(x + 1, y, color);
			draw_block(x + 2, y, color);
			break;
	}
}

void clear_piece(uint16_t x, uint16_t y, uint16_t piece) { draw_piece(x, y, WHITE, piece); }

uint8_t move_piece_down(uint16_t x, uint16_t y, uint16_t piece, Color_t color) {
	clear_piece(x, y, piece);
	draw_piece(x, y + 1, color, piece);
}

void tetris_thread(void* par) {
	for (int i = 0; i < MAX_X; i++) {
		for (int j = 0; j < MAX_Y; j++) {
			tetris_game_board[i][j] = 0;
		}
	}

	draw_game_window();

	draw_piece(0, 3, GRAY, 4);
	int x = 0;
	while (1) {
		osDelay(4);

		for (int i = 0; i < MAX_X; i++) {
			for (int j = 0; j < MAX_Y; j++) {
				tft_prints(i, j, "%d", tetris_game_board[i][j]);
			}
		}
		tft_update();
		move_piece_down(0, x, GRAY, 4);
	}
}