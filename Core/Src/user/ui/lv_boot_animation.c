// #include "lv_boot_animation.h"

// #include "stm32f4xx_hal.h"

// #include <stdio.h>

// lv_obj_t* bar = NULL;

// static void set_value(void* bar, int32_t v) { lv_bar_set_value(bar, v, LV_ANIM_OFF); }

// void clear_bar() {
// 	lv_obj_del(bar);
// }

// void lv_boot_animation(void (*end_cb)(lv_anim_t*), uint32_t boot_time) {
// 	static lv_style_t style_logo;
// 	static lv_style_t style_bar_bg;
// 	static lv_style_t style_indic;

// 	/**/
// 	lv_style_init(&style_logo);
// 	lv_style_set_radius(&style_logo, 20);
// 	lv_style_set_bg_opa(&style_logo, 0);
// 	lv_style_set_bg_color(&style_logo, lv_color_hex(0xffffff));
// 	lv_style_set_bg_grad_color(&style_logo, lv_color_hex(0xffffff));
// 	lv_style_set_bg_grad_dir(&style_logo, LV_GRAD_DIR_VER);

// 	lv_style_set_border_opa(&style_logo, LV_OPA_0);
// 	lv_style_set_border_width(&style_logo, 2);
// 	lv_style_set_border_color(&style_logo, lv_palette_main(LV_PALETTE_GREY));

// 	lv_style_set_outline_opa(&style_logo, LV_OPA_COVER);
// 	lv_style_set_outline_color(&style_logo, lv_palette_main(LV_PALETTE_GREEN));

// 	lv_style_set_text_color(&style_logo, lv_color_white());
// 	lv_style_set_pad_all(&style_logo, 10);

// 	/*Init the pressed style*/
// 	static lv_style_t style_pr;
// 	lv_style_init(&style_pr);

// 	/*Ad a large outline when pressed*/
// 	lv_style_set_outline_width(&style_pr, 30);
// 	lv_style_set_outline_opa(&style_pr, LV_OPA_TRANSP);

// 	lv_style_set_translate_y(&style_pr, 5);
// 	lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_GREEN, 2));
// 	lv_style_set_bg_grad_color(&style_pr, lv_palette_darken(LV_PALETTE_GREEN, 4));

// 	/*Add a transition to the the outline*/
// 	static lv_style_transition_dsc_t trans;
// 	static lv_style_prop_t props[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA, 0};
// 	lv_style_transition_dsc_init(&trans, props, lv_anim_path_linear, 300, 0, NULL);
// 	lv_style_set_transition(&style_pr, &trans);

// 	/**/
// 	lv_style_init(&style_bar_bg);
// 	lv_style_set_border_color(&style_bar_bg, lv_palette_main(LV_PALETTE_BLUE));
// 	lv_style_set_border_width(&style_bar_bg, 2);
// 	lv_style_set_pad_all(&style_bar_bg, 5); /*To make the indicator smaller*/
// 	lv_style_set_radius(&style_bar_bg, 6);

// 	lv_style_init(&style_indic);
// 	lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
// 	lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
// 	lv_style_set_radius(&style_indic, 3);

// 	bar = lv_bar_create(lv_scr_act());
// 	lv_obj_add_style(bar, &style_bar_bg, 0);
// 	lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
// 	lv_obj_set_size(bar, LV_PCT(40), 15);
// 	lv_obj_align(bar, LV_ALIGN_CENTER, 0, 20);


// 	lv_anim_t a_bar;
// 	lv_anim_init(&a_bar);
// 	lv_anim_set_var(&a_bar, bar);
// 	lv_anim_set_values(&a_bar, 0, 100);
// 	lv_anim_set_exec_cb(&a_bar, set_value);
// 	lv_anim_set_ready_cb(&a_bar, clear_bar);
// 	lv_anim_set_time(&a_bar, boot_time);
// 	lv_anim_start(&a_bar);
// }