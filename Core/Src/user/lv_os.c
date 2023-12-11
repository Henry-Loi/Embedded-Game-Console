#include "lv_os.h"

#include "lcd.h"
#include "lv_color.h"
#include "lv_disp.h"
#include "lv_img.h"
#include "lv_obj_style.h"
#include "lv_style.h"
#include "main.h"

// #include "lv_conf.h"
// #include "lvgl/lvgl.h"

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

static void lv_timer_update_time(lv_timer_t* timer) {
	lv_obj_t* label = timer->user_data;

	// get system time


	lv_label_set_text_fmt(label, "   %02d:%02d  %02d-%02d-%02d", ((int)get_ticks() / 60000) % 50,
						  (int)get_ticks() / 60000, ((int)get_ticks() / 1000) % 60, 11, 9);
}

static void lcd_top_widgets(lv_obj_t* parent) {
	static lv_style_t obj_layout_style; // 容器的样式

	lv_style_init(&obj_layout_style);
	lv_style_set_pad_all(&obj_layout_style, 0);
	lv_style_set_bg_opa(&obj_layout_style, 0);
	lv_style_set_text_font(&obj_layout_style, LV_FONT_DEFAULT);
	lv_style_set_border_opa(&obj_layout_style, 0);
	lv_style_set_radius(&obj_layout_style, 0);
	lv_style_set_text_color(&obj_layout_style, lv_color_hex(0xffffff));

	/* Layout Init */
	lv_obj_t* panel = lv_obj_create(parent);
	lv_obj_set_size(panel, LV_PCT(100), 30);
	lv_obj_add_style(panel, &obj_layout_style, 0);
	lv_obj_align(panel, LV_ALIGN_TOP_MID, 0, 5);

	/* 右上角小图标 */
	lv_obj_t* panel_icon = lv_obj_create(panel);
	lv_obj_set_size(panel_icon, 200, 25);
	lv_obj_set_layout(panel_icon, LV_LAYOUT_FLEX);
	lv_obj_set_style_base_dir(panel_icon, LV_BASE_DIR_RTL, 0);
	lv_obj_set_flex_flow(panel_icon, LV_FLEX_FLOW_ROW);
	lv_obj_align(panel_icon, LV_ALIGN_RIGHT_MID, 0, 0);
	lv_obj_add_style(panel_icon, &obj_layout_style, 0);

	lv_obj_t* label = lv_label_create(panel_icon);
	lv_label_set_text(label, " ");

	lv_obj_t* label_bat = lv_label_create(panel_icon);
	lv_label_set_text(label_bat, LV_SYMBOL_BATTERY_EMPTY);

	lv_obj_t* label_batchar = lv_label_create(label_bat);
	lv_obj_set_style_text_font(label_batchar, &lv_font_montserrat_14, 0);
	lv_label_set_text(label_batchar, LV_SYMBOL_CHARGE);
	lv_obj_center(label_batchar);


	lv_obj_t* label_wifi = lv_label_create(panel_icon);
	lv_label_set_text(label_wifi, LV_SYMBOL_WIFI);

	lv_obj_t* label_time = lv_label_create(panel);
	lv_label_set_text(label_time, "  ");
	lv_obj_align(label_time, LV_ALIGN_LEFT_MID, 0, 0);

	lv_timer_t* timer = lv_timer_create(lv_timer_update_time, 1000, label_time);
}

static void set_menu_table_tips(lv_obj_t* parent, int count) {
	lv_obj_t* round[3];

	/* 左菜单指示 */
	round[0] = lv_obj_create(parent); // 左边
	lv_obj_set_style_border_opa(round[0], 0, 0);
	lv_obj_set_size(round[0], MENU_TABLE_TIPS_PROMPT, MENU_TABLE_TIPS_PROMPT);
	lv_obj_align(round[0], LV_ALIGN_CENTER, -20, 180);

	round[1] = lv_obj_create(parent); // 中间
	lv_obj_set_style_border_opa(round[1], 0, 0);
	lv_obj_set_size(round[1], MENU_TABLE_TIPS_PROMPT, MENU_TABLE_TIPS_PROMPT);
	lv_obj_align(round[1], LV_ALIGN_CENTER, 0, 180);

	round[2] = lv_obj_create(parent); // 右边
	lv_obj_set_style_border_opa(round[2], 0, 0);
	lv_obj_set_size(round[2], MENU_TABLE_TIPS_PROMPT, MENU_TABLE_TIPS_PROMPT);
	lv_obj_align(round[2], LV_ALIGN_CENTER, 20, 180);

	for (int i = 0; i < 3; i++) {
		if (i == count)
			lv_obj_set_style_bg_opa(round[i], LV_OPA_80, 0);
		else
			lv_obj_set_style_bg_opa(round[i], LV_OPA_30, 0);
	}
}


static uint16_t g_tab_act = 1; // 更新tabview的索引，用于返回桌面时保持原来的位置
static void get_tab_act_index_event_handler(lv_event_t* e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t* obj = lv_event_get_target(e);

	if (code == LV_EVENT_VALUE_CHANGED) {
		if (obj) {
			g_tab_act = lv_tabview_get_tab_act(obj);
		} else
			g_tab_act = 1;
	}
}

void lv_example(void) {
	static lv_style_t style_logo;
	static lv_style_t style_bar_bg;
	static lv_style_t style_indic;


	lv_style_init(&style_logo);
	lv_style_set_radius(&style_logo, 20);
	lv_style_set_bg_opa(&style_logo, 0);
	lv_style_set_bg_color(&style_logo, lv_color_hex(0xffffff));
	lv_style_set_bg_grad_color(&style_logo, lv_color_hex(0xffffff));
	lv_style_set_bg_grad_dir(&style_logo, LV_GRAD_DIR_VER);

	lv_style_set_border_opa(&style_logo, LV_OPA_0);
	lv_style_set_border_width(&style_logo, 2);
	lv_style_set_border_color(&style_logo, lv_palette_main(LV_PALETTE_GREY));

	lv_style_set_outline_opa(&style_logo, LV_OPA_COVER);
	lv_style_set_outline_color(&style_logo, lv_palette_main(LV_PALETTE_GREEN));

	lv_style_set_text_color(&style_logo, lv_color_white());
	lv_style_set_pad_all(&style_logo, 10);
}
