#pragma once

#include "sd_diskio.h"
#include "tft.h"

#define USE_LVGL_DISPLAY 1

#define TFT_PAGE_TABLE(X) \
	X(Pong_Page)          \
	X(Asteroids_Page)     \
	X(Tetris_Page)        \
	X(Main_Page)          \
	X(Debug_Page)

#define TFT_PAGE_ENUM(X) X,
#define TFT_PAGE_NAME(X) #X,

typedef enum { TFT_PAGE_TABLE(TFT_PAGE_ENUM) NUM_OF_PAGE, NUM_OF_GAME = 3 } TFT_Display_Screen;

static char* tft_page_names[NUM_OF_PAGE] = {TFT_PAGE_TABLE(TFT_PAGE_NAME)};

#define DARKSCREEN_MODE_TIMEOUT (30 * 1000)
#define GARYSCREEN_MODE_TIMEOUT (5 * 1000)

int touch_screen_test(int r);
void tft_inactive_handler(void);
DSTATUS SD_status(BYTE);

extern TFT_Display_Screen lcd_screen;
