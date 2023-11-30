#include "tft.h"

#include "board.h"
#include "controller.h"
#include "diskio.h"
#include "display/lcd.h"
#include "display/touch.h"
#include "display/touch_btns.h"
#include "fatfs_platform.h"
#include "icm20602.h"
#include "lcd.h"
#include "lv_conf.h"
#include "lv_demo_widgets.h"
#include "lv_os.h"
#include "main.h"
#include "os.h"
#include "sd_diskio.h"
#include "ui/lv_boot_animation.h"
#include "ui/navbar.h"

#define USE_OWN_TFT				1
#define DARKSCREEN_MODE_TIMEOUT (30 * 1000)
#define GRAYSCREEN_MODE_TIMEOUT (5 * 1000)

// touch point testing code
int touch_screen_test(int r) {
	for (int i = 0; i < MAX_TOUCH_POINTS; i++) {
		tft_prints(0, r++, "POINT %d: ", i + 1);
		tft_prints(0, r++, "x: %d", touch_feedback.point[i].x);
		tft_prints(0, r++, "y: %d", touch_feedback.point[i].y);
	}
	tft_prints(0, r++, "NUM OF PRESSES: %d", touch_feedback.num);
	tft_prints(0, r++, "CLICKED : %d", touch_feedback.clicked);
	tft_prints(0, r++, "PRESSED : %d", touch_feedback.pressed_state);
	return r;
}

DSTATUS SD_status(BYTE);

Page_t curr_page = HOME_PAGE;
int brightness = 255;
void lcd_thread(void* par) {
	// lcd init
	// lcd_init();
	touch_init();

	QueueHandle_t MutexSemaphore = xSemaphoreCreateMutex();
	/*
		if (!USE_OWN_TFT) {
			// lvgl init function calls
			lv_init();
			lv_port_disp_init();
			lv_port_indev_init();

			lv_boot_animation(lv_init_icon, 5000);

			uint32_t last_ticks = get_ticks();
			// hold everything else until bar renders
			while (get_ticks() - last_ticks < 5000) {
				xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
				lv_task_handler();
				xSemaphoreGive(MutexSemaphore);
			}

			// actual stuff
			render_navbar();
		}
	 */
	while (1) {
		osDelay(4);

		/* 		if (get_ticks() - ctrller.inactive_count > DARKSCREEN_MODE_TIMEOUT) {
					tft_backlight_control(0);
				} else {
					tft_backlight_control(255);
				}
		 */
		if (get_ticks() - ctrller.inactive_count > DARKSCREEN_MODE_TIMEOUT) {
			tft_backlight_control(0);
		} else if (get_ticks() - ctrller.inactive_count > GRAYSCREEN_MODE_TIMEOUT) {
			tft_backlight_control(120);
		} else {
			tft_backlight_control(brightness);
		}


		if (curr_page == HOME_PAGE) {
			tft_prints(26, 8, "PONG");
			tft_prints(26, 12, "TETRIS");
			tft_prints(26, 16, "ASTEROIDS");
			tft_prints(20, LCD_MAX_CHAR_HEIGHT - 1, "<     Brightness     >");

			TouchBtn_t pong_btn = {.x = 25 * LCD_CHAR_SPACING_WIDTH,
								   .y = 8 * LCD_CHAR_SPACING_HEIGHT,
								   .x_len = 140,
								   .y_len = 30,
								   .label = "PONG",
								   .border_color = WHITE};

			TouchBtn_t tetris_btn = {.x = 25 * LCD_CHAR_SPACING_WIDTH,
									 .y = 12 * LCD_CHAR_SPACING_HEIGHT,
									 .x_len = 140,
									 .y_len = 30,
									 .label = "TETRIS",
									 .border_color = WHITE};

			TouchBtn_t asteroids_btn = {.x = 25 * LCD_CHAR_SPACING_WIDTH,
										.y = 16 * LCD_CHAR_SPACING_HEIGHT,
										.x_len = 170,
										.y_len = 30,
										.label = "ASTEROIDS",
										.border_color = WHITE};

			TouchBtn_t dec_brightness_btn = {.x = 19 * LCD_CHAR_SPACING_WIDTH,
											 .y = 23 * LCD_CHAR_SPACING_HEIGHT,
											 .x_len = 50,
											 .y_len = 30,
											 .label = "inc_brightness",
											 .border_color = RED};

			TouchBtn_t inc_brightness_btn = {.x = 40 * LCD_CHAR_SPACING_WIDTH,
											 .y = 23 * LCD_CHAR_SPACING_HEIGHT,
											 .x_len = 50,
											 .y_len = 30,
											 .label = "dec_brightness",
											 .border_color = RED};


			render_button(tetris_btn);
			render_button(pong_btn);
			render_button(asteroids_btn);
			render_button(inc_brightness_btn);
			render_button(dec_brightness_btn);
			if (detect_button_press(tetris_btn)) {
				tft_clear(WHITE);
				curr_page = TETRIS_PAGE;
			}

			if (detect_button_press(pong_btn)) {
				tft_clear(BLACK);
				curr_page = PONG_PAGE;
			}

			if (detect_button_press(asteroids_btn)) {
				tft_clear(BLACK);
				curr_page = ASTEROIDS_PAGE;
			}
			if (detect_button_press(inc_brightness_btn)) {
				brightness += 1;
				if (brightness >= 255) {
					brightness = 255;
				}
			}
			if (detect_button_press(dec_brightness_btn)) {
				brightness -= 1;
				if (brightness <= 0) {
					brightness = 0;
				}
			}
		}

		/* if (!USE_OWN_TFT) {
			// lv task handler
			xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
			lv_task_handler();
			xSemaphoreGive(MutexSemaphore);
		} else { */
		// int r = 0;
		// tft_prints(0, r++, "%02d:%02d.%02d", (int)get_ticks() / 60000, ((int)get_ticks() / 1000) % 60,
		//    ((int)get_ticks() % 1000) / 10);

		// r++;
		// r = touch_screen_test(r);
		// r++;

		// r++;
		// r = controller_tft(r);
		// r++;
		// r = imu_tft(r);
		// r++;
		// r = fatfs_tft(r);

		// tft_update();
		//}
		touch_update();
		tft_update();
	}
}
/*  */