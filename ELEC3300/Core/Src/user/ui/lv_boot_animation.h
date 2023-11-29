#pragma once

#include "lvgl/lvgl.h"

#include <stdint.h>

#define ICON_PATH			   ("./img/")
#define BG_IMG_NAME			   ("bg.png")
#define TAB_LEFT_APP_COUNT	   (4)	// APP的直接个数
#define TAB_MAIN_APP_COUNT	   (11) // TAB_LEFT_APP_COUNT至TAB_MAIN_APP_COUNT之间APP的放在这里
#define TAB_RIGHT_APP_COUNT	   (11) // 剩下的APP都放到这里(大于TAB_MAIN_APP_COUNT)
#define MENU_TABLE_TIPS_PROMPT (10) // 菜单位置提示大小

#define ICON_SIZE		  (64)
#define ICON_ROW_COUNT	  (4)
#define ICON_COLUNM_COUNT (6)
#define ICON_PAD_TOP	  (40)
#define ICON_PAD_BOTTOM	  (40)
#define ICON_PAD_LEFT	  (115)
#define ICON_PAD_RIGHT	  (115)

#define ICON_ROW_SPACE (60)
#define ICON_COL_SPACE (90) //((ICON_HOR_RES - (ICON_SIZE * ICON_COLUNM_COUNT)) / (ICON_COLUNM_COUNT - 1))
#define ICON_HOR_RES                     \
	(4 + (ICON_SIZE * ICON_COLUNM_COUNT) \
	 + (ICON_COL_SPACE * (ICON_COLUNM_COUNT - 1))) //((LV_HOR_RES - ICON_PAD_LEFT - ICON_PAD_RIGHT))        // 列间距
#define ICON_VER_RES                  \
	(4 + (ICON_SIZE * ICON_ROW_COUNT) \
	 + (ICON_ROW_SPACE * (ICON_ROW_COUNT - 1))) //((LV_VER_RES - ICON_PAD_TOP  - ICON_PAD_BOTTOM))       // 行间距

void lv_init_icon(lv_anim_t*);
void lv_boot_animation(void (*end_cb)(lv_anim_t*), uint32_t boot_time);
