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

#include "tetris_game.h"

#include "pieces.h"
#include "score.h"
#include "screen.h"

#include <math.h>
#include <string.h>

static struct piece current_piece;
static struct piece next_piece;

/* delay to wait for input (milliseconds) */
int tetris_delay;

static void add_current_piece(void) {
	int y, x;

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			if (tetris[current_piece.piece][current_piece.rotation][y][x])
				screen[current_piece.position.y - y][current_piece.position.x + x] = current_piece.piece + 1;
		}
	}
}

static void remove_current_piece(void) {
	int y, x;

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			if (tetris[current_piece.piece][current_piece.rotation][y][x])
				screen[current_piece.position.y - y][current_piece.position.x + x] = 0;
		}
	}
}

static int check_piece_overlap(void) {
	int y, x;

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			if (tetris[current_piece.piece][current_piece.rotation][y][x]
				&& (current_piece.position.y - y >= MAX_Y || current_piece.position.y - y < 0
					|| current_piece.position.x + x >= MAX_X || current_piece.position.x + x < 0
					|| screen[current_piece.position.y - y][current_piece.position.x + x]))
				return 1;
		}
	}
	return 0;
}

void start_new_game(void) {
	memset(screen, 0, sizeof(screen));
	current_piece = get_random_piece();
	next_piece = get_random_piece();
	tetris_score = 0;
	level = 1;
	add_current_piece();
}

static int eliminate_lines(void) {
	int lines_eliminated = 0;
	int y, x, h, k;

	/* calculate points based on number of lines eliminated */
	static const int points_per_line[] = {1, 40, 100, 300, 1200};

	/* search a line to eliminate */
	for (y = 0; y < MAX_Y; y++) {
		/* search if the line is complete */
		for (x = 0; x < MAX_X && screen[y][x]; x++)
			;

		if (x != MAX_X) /* line not complete, search next */
			continue;

		/* eliminate line i and move all superior lines down */
		for (h = y; h > 2; h--) {
			for (k = 0; k < MAX_X; k++) {
				screen[h][k] = screen[h - 1][k];
			}
		}

		lines_eliminated++;
	}


	return points_per_line[lines_eliminated];
}

static void handle_piece_bottom(void) {
	tetris_score += eliminate_lines();

	/* calculate new level from score */
	level = 1 + tetris_score / 700;

	/* Swaps the current piece whith the next one */
	current_piece = next_piece;
	next_piece = get_random_piece();

	if (check_piece_overlap()) {
		prompt_new_game();
		return;
	}

	add_current_piece();
}

static int do_move_down(void) {
	int bottom = 0;

	remove_current_piece();
	current_piece.position.y++;
	if (check_piece_overlap()) { /* cannot move down */
		bottom = 1;
		current_piece.position.y--;
	}
	add_current_piece();

	tetris_delay = 800 * pow(0.9, level);

	return bottom;
}

void move_down(void) {
	if (do_move_down())
		handle_piece_bottom();
}

void move_bottom(void) {
	while (!do_move_down())
		continue;
	handle_piece_bottom();
}

void move_left(void) {
	remove_current_piece();
	current_piece.position.x--;
	if (check_piece_overlap())
		current_piece.position.x++;
	add_current_piece();
}

void move_right(void) {
	remove_current_piece();
	current_piece.position.x++;
	if (check_piece_overlap())
		current_piece.position.x--;
	add_current_piece();
}

void rotate(void) {
	remove_current_piece();
	current_piece.rotation = (current_piece.rotation + 1) % 4;
	if (check_piece_overlap())
		current_piece.rotation = (current_piece.rotation - 1) % 4;
	add_current_piece();
}


void rotate2(void) {
	remove_current_piece();
	current_piece.rotation = (current_piece.rotation - 1 + 4) % 4;
	if (check_piece_overlap())
		current_piece.rotation = (current_piece.rotation + 1) % 4;
	add_current_piece();
}

void get_next_piece(char next[4][4]) {
	int y, x;

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			next[y][x] = tetris[next_piece.piece][next_piece.rotation][y][x] ? next_piece.piece + 1 : 0;
		}
	}
}
