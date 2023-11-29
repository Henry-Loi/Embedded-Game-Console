#include "navbar.h"

#include "../../lv_examples.h"
#include "games/tictactoe/tictactoe.h"
#include "lcd.h"
#include "lv_area.h"
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

static lv_obj_t* slider_label;
static void slider_event_cb(lv_event_t* e) {
	lv_obj_t* slider = lv_event_get_target(e);
	char buf[8];
	lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
	tft_backlight_control((int)lv_slider_get_value(slider) * 2.55);
	lv_label_set_text(slider_label, buf);
	lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

void lvgl_main_page(void) {
	/*Create a slider in the center of the display*/
	lv_obj_t* slider = lv_slider_create(lv_scr_act());
	lv_obj_center(slider);
	lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
	lv_obj_align(slider, LV_ALIGN_BOTTOM_MID, 0, -50);

	/*Create a label below the slider*/
	slider_label = lv_label_create(lv_scr_act());
	lv_slider_set_value(slider, tft_get_brightness(), LV_ANIM_OFF);
	char buf[8];
	lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
	lv_label_set_text(slider_label, buf);

	lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

	/*Create a container with COLUMN flex direction*/
	lv_obj_t* cont_col = lv_obj_create(lv_scr_act());
	lv_obj_set_size(cont_col, 340, 180);
	lv_obj_align_to(cont_col, slider, LV_ALIGN_OUT_TOP_MID, 0, -70);
	lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);

	uint32_t i;
	for (i = 0; i < NUM_OF_GAME; i++) {
		lv_obj_t* obj;
		lv_obj_t* label;

		/*Add items to the column*/
		obj = lv_btn_create(cont_col);
		lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);

		label = lv_label_create(obj);
		lv_label_set_text_fmt(label, "%s", tft_page_names[i]);
		lv_obj_center(label);
	}

	LV_IMG_DECLARE(img_cogwheel_argb);
	lv_obj_t* img1 = lv_img_create(lv_scr_act());
	lv_img_set_src(img1, &img_cogwheel_argb);
	lv_obj_align(img1, LV_ALIGN_TOP_RIGHT, -2, 2);
	lv_obj_set_size(img1, 100, 100);
	lv_img_set_zoom(img1, 150);
}
