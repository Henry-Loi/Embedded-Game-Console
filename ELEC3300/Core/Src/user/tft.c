#include "controller.h"
#include "display/lcd.h"
#include "display/touch.h"
#include "lcd.h"
#include "lv_conf.h"
#include "lv_os.h"
#include "os.h"
#include "ui/lv_boot_animation.h"
#include "ui/navbar.h"

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

void print_line() {
	static lv_point_t line_points[] = {{5, 5}, {70, 70}, {120, 10}, {180, 60}, {240, 10}};

	/*Create style*/
	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, 8);
	lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
	lv_style_set_line_rounded(&style_line, true);

	/*Create a line and apply the new style*/
	lv_obj_t* line1;
	line1 = lv_line_create(lv_scr_act());
	lv_line_set_points(line1, line_points, 5); /*Set the points*/
	lv_obj_add_style(line1, &style_line, 0);
	lv_obj_center(line1);
}

void lcd_thread(void* par) {
	// lcd init
	lcd_init();
	touch_init();

	// lvgl init function calls
	lv_init();
	lv_port_disp_init();  /* lvgl lcd display init */
	lv_port_indev_init(); /* lvgl lcd touch screen init */
	QueueHandle_t MutexSemaphore = xSemaphoreCreateMutex();

	// lv_boot_animation(lv_init_icon, 5000);

	uint32_t last_ticks = get_ticks();
	// while (get_ticks() - last_ticks < 5000) {
	// 	xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
	// 	lv_task_handler();
	// 	xSemaphoreGive(MutexSemaphore);
	// }

	// test touch btn
	// lv_example();
	render_navbar();


	while (1) {
		osDelay(4);

		// lv task handler
		xSemaphoreTake(MutexSemaphore, portMAX_DELAY);
		lv_task_handler();
		xSemaphoreGive(MutexSemaphore);

		// int r = 5;
		// tft_prints(0, r++, "%02d:%02d.%02d", (int)get_ticks() / 60000, ((int)get_ticks() / 1000) % 60,
		// 		   ((int)get_ticks() % 1000) / 10);

		// r = touch_screen_test(r);
		// r = controller_tft(r);

		// tft_update();
		touch_update();
	}
}
