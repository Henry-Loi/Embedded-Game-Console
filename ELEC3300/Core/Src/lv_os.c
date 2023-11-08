#include "lv_os.h"

#include "lv_port_indev.h"

/* Example Testing code for touch screen */
void btn_event_cb(lv_event_t* e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* btn = lv_event_get_target(e);
	if (code == LV_EVENT_CLICKED) {
		static uint8_t cnt = 0;
		cnt++;
		lv_obj_t* label = lv_obj_get_child(btn, 0);
		lv_label_set_text_fmt(label, "Button: %d", cnt);
	}
}

void lv_example(void) {
	lv_obj_t* btn = lv_btn_create(lv_scr_act());
	lv_obj_set_pos(btn, 10, 10);
	lv_obj_set_size(btn, 300, 100);
	lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_t* label = lv_label_create(btn);
	lv_label_set_text(label, "Button");
	lv_obj_center(label);
}
