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

#ifndef SCREEN_H
#define SCREEN_H

#define TOP_PADDING 5

#define MAX_X 22
#define MAX_Y 19

extern int screen[MAX_Y][MAX_X];

void prompt_new_game(void);

void tetris_thread(void* par);
#endif /* SCREEN_H */
