#include "navbar.h"

#include "games/tictactoe/tictactoe.h"
#include "lvgl/lvgl.h"
#include "tft.h"

// TODO: move this up?

void render_navbar(void) {
	lv_obj_t* tictactoe_btn = lv_btn_create(lv_scr_act());
	lv_obj_set_pos(tictactoe_btn, 400, 15);
	lv_obj_set_size(tictactoe_btn, 200, 50);
	lv_obj_add_event_cb(tictactoe_btn, toggle_tictactoe, LV_EVENT_CLICKED, NULL);
	lv_obj_t* tictactoe_label = lv_label_create(tictactoe_btn);
	lv_label_set_text(tictactoe_label, "Start TicTacToe");
	lv_obj_center(tictactoe_label);
}

void lvgl_main_page(void) {
	/*Create a container with ROW flex direction*/
	lv_obj_t* cont_row = lv_obj_create(lv_scr_act());
	lv_obj_set_size(cont_row, 300, 75);
	lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 5);
	lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);

	/*Create a container with COLUMN flex direction*/
	lv_obj_t* cont_col = lv_obj_create(lv_scr_act());
	lv_obj_set_size(cont_col, 200, 150);
	lv_obj_align_to(cont_col, cont_row, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
	lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);

	uint32_t i;
	for (i = 0; i < NUM_OF_GAME; i++) {
		lv_obj_t* obj;
		lv_obj_t* label;

		/*Add items to the row*/
		obj = lv_btn_create(cont_row);
		lv_obj_set_size(obj, 100, LV_PCT(100));

		label = lv_label_create(obj);
		lv_label_set_text_fmt(label, "Item: %u", i);
		lv_obj_center(label);

		/*Add items to the column*/
		obj = lv_btn_create(cont_col);
		lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);

		label = lv_label_create(obj);
		lv_label_set_text_fmt(label, "%s", tft_page_names[i]);
		lv_obj_center(label);
	}
}
