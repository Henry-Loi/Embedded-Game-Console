#include "lv_boot_animation.h"

#include "stm32f4xx_hal.h"

#include <stdio.h>

static void lv_timer_update_time(lv_timer_t* timer) {
	lv_obj_t* label = timer->user_data;

	lv_label_set_text_fmt(label, "   %02d:%02d  %02d-%02d-%02d", (int)HAL_GetTick() / 60000,
						  ((int)HAL_GetTick() / 1000) % 60, 11, 7, 2023);
}

static void lcd_top_widgets(lv_obj_t* parent) {
	static lv_style_t obj_layout_style; // 容器的样式

	lv_style_init(&obj_layout_style);
	lv_style_set_pad_all(&obj_layout_style, 0);
	lv_style_set_bg_opa(&obj_layout_style, 0);
	lv_style_set_text_font(&obj_layout_style, &lv_font_montserrat_14);
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

void lv_init_icon(void) {
	char bg_path_name[128];	  // 存放桌面背景路径的缓冲区
	char icon_path_name[128]; // 存放APP图标路径的缓冲区
	int app_count = 1;		  // 计数以区分不同的tab页面

	lv_obj_t* img_gb;		   // 桌面背景
	lv_obj_t* img_icon;		   // APP图标
	lv_obj_t* label_icon;	   // 基于APP图标创建的名称，点击时图标时提取，不展示出来
	lv_obj_t* label_icon_name; // 展示在图标下方的名称
	lv_obj_t* tabview_desktop; // tab总页面
	lv_obj_t* img_bottom_icon; // 展示在底部快速访问栏的图标
	lv_obj_t* label_bottom_icon; // 基于底部快速访问栏的图标创建的名称，点击时图标时提取，不展示出来

	lv_obj_t* tab_left;		   // 左边的tab页面
	lv_obj_t* tab_main;		   // 中间的tab页面
	lv_obj_t* tab_right;	   // 右边的tab页面
	lv_obj_t* icon_cont_left;  // 中间图标区域面板
	lv_obj_t* icon_cont_main;  // 中间图标区域面板
	lv_obj_t* icon_cont_right; // 中间图标区域面板

	static lv_style_t style_tabview_desktop;  // 容器的样式
	static lv_style_t cont_style;			  // 中间图标区域，容器的样式
	static lv_style_t icon_style;			  // 中间图标区域，容器中的图标的样式
	static lv_style_t obj_bottom_panel_style; // 底部容器的样式

	if (lv_obj_get_child(lv_scr_act(), 0))
		lv_obj_del(lv_obj_get_child(lv_scr_act(), 0));
	lv_obj_t* screen = lv_obj_create(NULL);
	lv_scr_load(screen);

	// lv_style_reset(&cont_style);
	// lv_style_reset(&icon_style);
	// lv_style_reset(&obj_bottom_panel_style);

	lv_style_init(&style_tabview_desktop);
	// lv_style_set_pad_all(&style_tabview_desktop, 0);
	lv_style_set_bg_opa(&style_tabview_desktop, 0);
	// lv_style_set_shadow_opa(&style_tabview_desktop, 0);
	// lv_style_set_border_opa(&style_tabview_desktop, 0);

	/* 设置容器的样式 */
	lv_style_init(&cont_style);
	lv_style_set_bg_opa(&cont_style, 0);
	lv_style_set_border_opa(&cont_style, 0);
	lv_style_set_pad_column(&cont_style, ICON_COL_SPACE);
	lv_style_set_pad_row(&cont_style, ICON_ROW_SPACE);
	lv_style_set_pad_all(&cont_style, 0);
	lv_style_set_layout(&cont_style, LV_LAYOUT_FLEX);
	lv_style_set_base_dir(&cont_style, LV_BASE_DIR_LTR);
	lv_style_set_flex_flow(&cont_style, LV_FLEX_FLOW_ROW_WRAP);

	/* 容器中的图标的样式 */
	lv_style_init(&icon_style);
	lv_style_set_text_opa(&icon_style, 0);
	lv_style_set_text_font(&icon_style, &lv_font_montserrat_14);
	// lv_style_set_radius(&icon_style, 0);
	// lv_style_set_border_width(&icon_style, 1);

	/* 底部面板区域 */
	lv_style_init(&obj_bottom_panel_style);
	lv_style_set_pad_all(&obj_bottom_panel_style, 0);
	lv_style_set_bg_opa(&obj_bottom_panel_style, LV_OPA_50);
	lv_style_set_pad_left(&obj_bottom_panel_style, 10);
	lv_style_set_pad_right(&obj_bottom_panel_style, 10);
	// lv_style_set_shadow_opa(&obj_bottom_panel_style, 0);
	lv_style_set_border_opa(&obj_bottom_panel_style, 0);
	lv_style_set_radius(&obj_bottom_panel_style, 22);

	// 桌面背景
	img_gb = lv_img_create(lv_scr_act());
	lv_snprintf(bg_path_name, sizeof(bg_path_name), "%s%s", ICON_PATH, BG_IMG_NAME);
	lv_img_set_src(img_gb, bg_path_name);

	/* 屏幕顶部状态栏区域 */
	lcd_top_widgets(lv_scr_act());

	/*Create a Tab view object*/
	// tabview_desktop = lv_tabview_create(lv_layer_top(), LV_DIR_TOP, 0);
	tabview_desktop = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0);
	lv_obj_add_style(tabview_desktop, &style_tabview_desktop, 0);
	lv_obj_add_event_cb(tabview_desktop, get_tab_act_index_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
	/*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
	tab_left = lv_tabview_add_tab(tabview_desktop, "left_desktop");
	tab_main = lv_tabview_add_tab(tabview_desktop, "main_desktop");
	tab_right = lv_tabview_add_tab(tabview_desktop, "right_desktop");
	lv_tabview_set_act(tabview_desktop, g_tab_act, LV_ANIM_OFF);

	/* 中间图标区域面板 */
	icon_cont_left = lv_obj_create(tab_left);
	lv_obj_set_size(icon_cont_left, ICON_HOR_RES, ICON_VER_RES);
	lv_obj_center(icon_cont_left);
	lv_obj_add_style(icon_cont_left, &cont_style, 0);

	icon_cont_main = lv_obj_create(tab_main);
	lv_obj_set_size(icon_cont_main, ICON_HOR_RES, ICON_VER_RES);
	lv_obj_center(icon_cont_main);
	lv_obj_add_style(icon_cont_main, &cont_style, 0);

	icon_cont_right = lv_obj_create(tab_right);
	lv_obj_set_size(icon_cont_right, ICON_HOR_RES, ICON_VER_RES);
	lv_obj_center(icon_cont_right);
	lv_obj_add_style(icon_cont_right, &cont_style, 0);

	/* 底部快速访问栏面板 */
	lv_obj_t* bottom_panel = lv_obj_create(lv_scr_act());
	lv_obj_set_size(bottom_panel, LV_PCT(70), 80);
	lv_obj_add_style(bottom_panel, &obj_bottom_panel_style, 0);
	lv_obj_set_layout(bottom_panel, LV_LAYOUT_FLEX);
	// lv_obj_set_style_base_dir(bottom_panel, LV_BASE_DIR_RTL, 0);
	lv_obj_set_flex_flow(bottom_panel, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(bottom_panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
	lv_obj_align(bottom_panel, LV_ALIGN_BOTTOM_MID, 0, -15);

	// 菜单位置提示
	set_menu_table_tips(tab_left, 0);
	set_menu_table_tips(tab_main, 1);
	set_menu_table_tips(tab_right, 2);
}

static void set_value(void* bar, int32_t v) { lv_bar_set_value(bar, v, LV_ANIM_OFF); }

void lv_boot_animation(void (*end_cb)(lv_anim_t*), uint32_t boot_time) {
	static lv_style_t style_logo;
	static lv_style_t style_bar_bg;
	static lv_style_t style_indic;

	/**/
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

	/*Init the pressed style*/
	static lv_style_t style_pr;
	lv_style_init(&style_pr);

	/*Ad a large outline when pressed*/
	lv_style_set_outline_width(&style_pr, 30);
	lv_style_set_outline_opa(&style_pr, LV_OPA_TRANSP);

	lv_style_set_translate_y(&style_pr, 5);
	lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_GREEN, 2));
	lv_style_set_bg_grad_color(&style_pr, lv_palette_darken(LV_PALETTE_GREEN, 4));

	/*Add a transition to the the outline*/
	static lv_style_transition_dsc_t trans;
	static lv_style_prop_t props[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA, 0};
	lv_style_transition_dsc_init(&trans, props, lv_anim_path_linear, 300, 0, NULL);
	lv_style_set_transition(&style_pr, &trans);

	/**/
	lv_style_init(&style_bar_bg);
	lv_style_set_border_color(&style_bar_bg, lv_palette_main(LV_PALETTE_BLUE));
	lv_style_set_border_width(&style_bar_bg, 2);
	lv_style_set_pad_all(&style_bar_bg, 5); /*To make the indicator smaller*/
	lv_style_set_radius(&style_bar_bg, 6);

	lv_style_init(&style_indic);
	lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
	lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
	lv_style_set_radius(&style_indic, 3);

	/* 主面板 */
	lv_obj_t* panel = lv_obj_create(lv_scr_act());
	lv_obj_set_size(panel, LV_PCT(100), LV_PCT(100));
	lv_obj_set_style_bg_color(panel, lv_color_hex(0xffffff), 0);
	lv_obj_set_style_radius(panel, 0, 0);

	lv_obj_t* bar = lv_bar_create(panel);
	lv_obj_add_style(bar, &style_bar_bg, 0);
	lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
	lv_obj_set_size(bar, LV_PCT(40), 15);
	lv_obj_align(bar, LV_ALIGN_CENTER, 0, 20);

	lv_obj_t* btn_logo = lv_btn_create(panel);
	lv_obj_set_size(btn_logo, 158, 41);
	lv_obj_remove_style_all(btn_logo); /*Remove the style coming from the theme*/
	lv_obj_add_style(btn_logo, &style_logo, 0);
	lv_obj_add_style(btn_logo, &style_pr, LV_STATE_PRESSED);
	// lv_obj_align_to(btn_logo, bar, LV_ALIGN_OUT_TOP_MID, 0, 0);
	lv_obj_align(btn_logo, LV_ALIGN_CENTER, 0, -30);


	lv_obj_t* img = lv_img_create(btn_logo);
	lv_img_set_src(img, "./bg.png");

	lv_anim_t a_bar;
	lv_anim_init(&a_bar);
	lv_anim_set_var(&a_bar, bar);
	lv_anim_set_values(&a_bar, 0, 100);
	lv_anim_set_exec_cb(&a_bar, set_value);
	lv_anim_set_ready_cb(&a_bar, end_cb);
	lv_anim_set_time(&a_bar, boot_time);
	// lv_anim_set_playback_time(&a_bar, 2000);
	lv_anim_start(&a_bar);
}