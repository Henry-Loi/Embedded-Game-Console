#include "navbar.h"

#include "games/tictactoe.h"
#include "lvgl/lvgl.h"

uint8_t shown = 0;

// TODO: move this up?
void toggle_tictactoe(lv_event_t* e) {
	if (!shown) {
		shown = 1;
		init_tictactoe();
		render_tictactoe_disp();
		lv_label_set_text(lv_obj_get_child(lv_event_get_target(e), 0), "Back");
	} else {
		shown = 0;
		clear_tictactoe_disp();
		lv_label_set_text(lv_obj_get_child(lv_event_get_target(e), 0), "Start TicTacToe");
	}
}

void render_navbar() {
	lv_obj_t* tictactoe_btn = lv_btn_create(lv_scr_act());
	lv_obj_set_pos(tictactoe_btn, 400, 15);
	lv_obj_set_size(tictactoe_btn, 200, 50);
	lv_obj_add_event_cb(tictactoe_btn, toggle_tictactoe, LV_EVENT_CLICKED, NULL);
	lv_obj_t* tictactoe_label = lv_label_create(tictactoe_btn);
	lv_label_set_text(tictactoe_label, "Start TicTacToe");
	lv_obj_center(tictactoe_label);
}