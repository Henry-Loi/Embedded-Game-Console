#include "tft.h"

#include "board.h"
#include "controller.h"
#include "diskio.h"
#include "display/lcd.h"
#include "display/touch.h"
#include "icm20602.h"
#include "lcd.h"
#include "lv_conf.h"
#include "lv_demo_widgets.h"
#include "lv_os.h"
#include "main.h"
#include "os.h"
#include "ui/lv_boot_animation.h"
#include "ui/navbar.h"

TFT_Display_Screen lcd_screen = Main_Page;
void lcd_thread(void* par) {
	// lcd init
	lcd_init();
	touch_init();

	QueueHandle_t MutexSemaphore = xSemaphoreCreateMutex();

	// power saving mode
	// tft_inactive_handler();

#ifdef USE_LVGL_DISPLAY
	// lvgl init function calls
	lv_init();
	lv_port_disp_init();  /* lvgl lcd display init */
	lv_port_indev_init(); /* lvgl lcd touch screen init */

	lv_boot_animation(lv_init_icon, 5000);

	uint32_t last_ticks = get_ticks();
	// hold everything else until bar renders
	while (get_ticks() - last_ticks < 5000) {
		xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
		lv_task_handler();
		xSemaphoreGive(MutexSemaphore);
	}

	// actual stuff
	// render_navbar();
	lvgl_main_page();
#endif

	while (1) {
		osDelay(4);
		int r = 0;

		// tft_prints(0, r++, "%s", tft_page_names[lcd_screen]);
		switch (lcd_screen) {
			case Main_Page:
#ifdef USE_LVGL_DISPLAY
				xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
				lv_task_handler();
				xSemaphoreGive(MutexSemaphore);
#endif
				break;
			case Debug_Page:
				tft_prints(0, r++, "%02d:%02d.%02d", (int)get_ticks() / 60000, ((int)get_ticks() / 1000) % 60,
						   ((int)get_ticks() % 1000) / 10);
				r++;
				r = touch_screen_test(r);
				r++;
				r = controller_tft(r);
				r++;
				r = imu_tft(r);
				// r++;
				// r = fatfs_tft(r);
				break;

			case Pong_Page: break;
			case Tetris_Page: break;
			case Asteroids_Page: break;
			default: break;
		}

		tft_update();
		touch_update();
	}
}


// touch point testing code
int touch_screen_test(int r) {
	for (int i = 0; i < MAX_TOUCH_POINTS; i++) {
		tft_prints(0, r++, "POINT %d: ", i + 1);
		tft_prints(0, r++, "x: %d", touch_feedback.point[i].x);
		tft_prints(0, r++, "y: %d", touch_feedback.point[i].y);
	}
	tft_prints(0, r++, "NUM OF PRESSES: %d", touch_feedback.pressed_state);
	return r;
}


void tft_inactive_handler(void) {
	if (get_ticks() - ctrller.inactive_count > DARKSCREEN_MODE_TIMEOUT) {
		tft_backlight_control(0);
	} else if (get_ticks() - ctrller.inactive_count > GARYSCREEN_MODE_TIMEOUT) {
		tft_backlight_control(120);
	} else {
		tft_backlight_control(255);
	}
}
