#include "controller.h"
#include "display/lcd.h"
#include "display/touch.h"
#include "lcd.h"
#include "lv_conf.h"
#include "lv_os.h"
#include "os.h"

// touch point testing code
int touch_screen_test(int r) {
	for (int i = 0; i < MAX_TOUCH_POINTS; i++) {
		tft_prints(0, r++, "Point %d: ", i);
		tft_prints(0, r++, "x: %d", touch_feedback.point[i].x);
		tft_prints(0, r++, "y: %d", touch_feedback.point[i].y);
	}
	tft_prints(0, r++, "test: %d", touch_feedback.pressed_state);
	return r;
}

void lcd_thread(void* par) {
	// lcd init
	lcd_init();
	touch_init();

	// lvgl init function calls
	lv_init();
	lv_port_disp_init();  /* lvgl lcd display init */
	lv_port_indev_init(); /* lvgl lcd touch screen init */

	// test touch btn
	lv_example();
	QueueHandle_t MutexSemaphore = xSemaphoreCreateMutex();

	while (1) {
		osDelay(4);

		// lv task handler
		xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
		lv_task_handler();
		xSemaphoreGive(MutexSemaphore);

		int r = 5;
		tft_prints(0, r++, "%02d:%02d.%02d", (int)get_ticks() / 60000, ((int)get_ticks() / 1000) % 60,
				   ((int)get_ticks() % 1000) / 10);

		r = touch_screen_test(r);
		r = controller_tft(r);

		tft_update();
		touch_update();
	}
}
