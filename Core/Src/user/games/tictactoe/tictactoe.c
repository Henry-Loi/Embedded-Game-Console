// #include "tictactoe.h"

// // TODO: don't hardcode everything? maybe also make it look better?

// LV_IMG_DECLARE(circle50px);
// LV_IMG_DECLARE(x50px);
// LV_IMG_DECLARE(empty);

// #define BOARD_SIZE 3
// #define LINE_NUM   4
// #define EMPTY	   ' '
// #define CIRCLE	   'O'
// #define CROSS	   'X'

// // game states
// char game_board[BOARD_SIZE][BOARD_SIZE] = {EMPTY};
// char curr_turn = CIRCLE;
// char victor = EMPTY;

// // ui stuff
// lv_obj_t* grid_lines[4] = {NULL};
// lv_point_t grid_points[] = {{400, 100}, {400, 500}, {600, 100}, {600, 500},
// 							{200, 230}, {800, 230}, {200, 370}, {800, 370}};
// lv_obj_t* grid_btns[BOARD_SIZE][BOARD_SIZE] = {NULL};
// lv_obj_t* grid_labels[BOARD_SIZE][BOARD_SIZE] = {NULL};
// lv_obj_t* grid_imgs[BOARD_SIZE][BOARD_SIZE] = {NULL};
// int grid_id[BOARD_SIZE * BOARD_SIZE] = {0};
// lv_obj_t* state_label = NULL;

// lv_style_t style_line;
// lv_style_t style_grid_btn;

// bool shown = 0;

// void toggle_tictactoe(lv_event_t* e) {
// 	if (!shown) {
// 		shown = 1;
// 		init_tictactoe();
// 		render_tictactoe_disp();
// 		lv_label_set_text(lv_obj_get_child(lv_event_get_target(e), 0), "Back");
// 	} else {
// 		shown = 0;
// 		clear_tictactoe_disp();
// 		lv_label_set_text(lv_obj_get_child(lv_event_get_target(e), 0), "Start TicTacToe");
// 	}
// }

// void init_tictactoe() {
// 	for (int i = 0; i < BOARD_SIZE; i++) {
// 		for (int j = 0; j < BOARD_SIZE; j++) {
// 			game_board[i][j] = EMPTY;
// 			grid_id[i * BOARD_SIZE + j] = i * BOARD_SIZE + j;
// 		}
// 	}
// 	curr_turn = CIRCLE;
// 	victor = EMPTY;
// }

// char check_endgame() {
// 	// rows
// 	for (int row = 0; row < BOARD_SIZE; row++) {
// 		if (game_board[row][0] == game_board[row][1] && game_board[row][1] == game_board[row][2]) {
// 			if (game_board[row][0] != EMPTY) {
// 				return game_board[row][0];
// 			}
// 		}
// 	}

// 	// columns
// 	for (int col = 0; col < 3; col++) {
// 		if (game_board[0][col] == game_board[1][col] && game_board[1][col] == game_board[2][col]) {
// 			if (game_board[0][col] != EMPTY) {
// 				return game_board[0][col];
// 			}
// 		}
// 	}

// 	// diagonals
// 	if (game_board[0][0] == game_board[1][1] && game_board[1][1] == game_board[2][2]) {
// 		if (game_board[0][0] != EMPTY) {
// 			return game_board[0][0];
// 		}
// 	}
// 	if (game_board[0][2] == game_board[1][1] && game_board[1][1] == game_board[2][0]) {
// 		if (game_board[0][2] != EMPTY) {
// 			return game_board[0][2];
// 		}
// 	}

// 	return EMPTY;
// }

// void grid_btns_cb(lv_event_t* e) {
// 	int curr_id = *((int*)lv_event_get_user_data(e));
// 	int row = curr_id / BOARD_SIZE;
// 	int col = curr_id % BOARD_SIZE;
// 	if (game_board[row][col] == EMPTY && victor == EMPTY) {
// 		game_board[row][col] = curr_turn;
// 		// lv_label_set_text_fmt(lv_obj_get_child(lv_event_get_target(e), 0), "%c", curr_turn);
// 		lv_img_set_src(lv_obj_get_child(lv_event_get_target(e), 0), curr_turn == CIRCLE ? &circle50px : &x50px);
// 		curr_turn = curr_turn == CIRCLE ? CROSS : CIRCLE;
// 	}

// 	victor = check_endgame();
// 	if (victor != EMPTY) {
// 		lv_label_set_text_fmt(state_label, "VICTOR: %s", victor == CIRCLE ? "CIRCLE" : "CROSS");
// 	} else {
// 		lv_label_set_text_fmt(state_label, "CURR TURN: %s", curr_turn == CIRCLE ? "CIRCLE" : "CROSS");
// 	}
// }

// void render_tictactoe_disp() {
// 	lv_style_init(&style_line);
// 	lv_style_set_line_width(&style_line, 8);
// 	lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_INDIGO));
// 	lv_style_set_line_rounded(&style_line, true);

// 	lv_style_init(&style_grid_btn);
// 	lv_style_set_bg_opa(&style_grid_btn, 0);

// 	for (int i = 0; i < LINE_NUM; i++) {
// 		grid_lines[i] = lv_line_create(lv_scr_act());
// 		lv_line_set_points(grid_lines[i], &grid_points[i * 2], 2);
// 		lv_obj_add_style(grid_lines[i], &style_line, 0);
// 	}

// 	for (int i = 0; i < BOARD_SIZE; i++) {
// 		for (int j = 0; j < BOARD_SIZE; j++) {
// 			grid_btns[i][j] = lv_btn_create(lv_scr_act());
// 			lv_obj_set_pos(grid_btns[i][j], 250 + j * 200, 100 + i * 150);
// 			lv_obj_set_size(grid_btns[i][j], 100, 100);
// 			lv_obj_add_event_cb(grid_btns[i][j], grid_btns_cb, LV_EVENT_CLICKED, &grid_id[i * BOARD_SIZE + j]);
// 			lv_obj_add_style(grid_btns[i][j], &style_grid_btn, 0);
// 			// grid_labels[i][j] = lv_label_create(grid_btns[i][j]);
// 			// lv_label_set_text(grid_labels[i][j], " ");
// 			// lv_obj_center(grid_labels[i][j]);

// 			grid_imgs[i][j] = lv_img_create(grid_btns[i][j]);
// 			lv_img_set_src(grid_imgs[i][j], &empty);
// 			lv_obj_center(grid_imgs[i][j]);
// 		}
// 	}

// 	state_label = lv_label_create(lv_scr_act());
// 	lv_label_set_text_fmt(state_label, "CURR TURN: %s", curr_turn == CIRCLE ? "CIRCLE" : "CROSS");
// 	lv_obj_set_pos(state_label, 450, 550);
// }

// void clear_tictactoe_disp() {
// 	for (int i = 0; i < LINE_NUM; i++) {
// 		lv_obj_del(grid_lines[i]);
// 		grid_lines[i] = NULL;
// 	}

// 	for (int i = 0; i < BOARD_SIZE; i++) {
// 		for (int j = 0; j < BOARD_SIZE; j++) {
// 			lv_obj_del(grid_btns[i][j]);
// 			grid_btns[i][j] = NULL;
// 			grid_labels[i][j] = NULL;
// 			grid_imgs[i][j] = NULL;
// 		}
// 	}
// 	lv_obj_del(state_label);
// 	state_label = NULL;
// }