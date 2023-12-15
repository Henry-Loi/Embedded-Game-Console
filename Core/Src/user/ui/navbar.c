// #include "navbar.h"

// #include "games/tictactoe/tictactoe.h"
// #include "lvgl/lvgl.h"

// // TODO: move this up?

// void render_navbar() {
// 	lv_obj_t* tictactoe_btn = lv_btn_create(lv_scr_act());
// 	lv_obj_set_pos(tictactoe_btn, 400, 15);
// 	lv_obj_set_size(tictactoe_btn, 200, 50);
// 	lv_obj_add_event_cb(tictactoe_btn, toggle_tictactoe, LV_EVENT_CLICKED, NULL);
// 	lv_obj_t* tictactoe_label = lv_label_create(tictactoe_btn);
// 	lv_label_set_text(tictactoe_label, "Start TicTacToe");
// 	lv_obj_center(tictactoe_label);
// }
