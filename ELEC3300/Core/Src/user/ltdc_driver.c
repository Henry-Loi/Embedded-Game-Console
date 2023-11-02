#include "ltdc_driver.h"

void lcd_backlight_control(uint8_t brightness) {
	if (brightness) {
		// turn on the backlight
		HAL_GPIO_WritePin(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, GPIO_PIN_SET);
	} else {
		// turn off the backlight
		HAL_GPIO_WritePin(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, GPIO_PIN_RESET);
	}
}

void lcd_clear(uint16_t color) {
	uint16_t* ptr = (uint16_t*)LCD_FRAME_BUFFER;
	uint32_t i = 0;

	while (i++ < LCD_WIDTH * LCD_HEIGHT) {
		*(ptr + i) = color;
	}
}

void lcd_init() {
	lcd_clear(WHITE);
	lcd_backlight_control(255);
}
