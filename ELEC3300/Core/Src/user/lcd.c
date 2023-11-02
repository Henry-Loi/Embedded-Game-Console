#include "lcd.h"

#include "dma2d.h"

void lcd_backlight_control(uint8_t brightness) {
	if (brightness) {
		HAL_GPIO_WritePin(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, GPIO_PIN_RESET);
	}
}

void lcd_clear(uint16_t color) {
#if USE_DMA2D_EN
	dma2d_transfer_data_r2m((uint32_t*)LCD_FRAME_BUFFER, LCD_WIDTH, LCD_HEIGHT, 0, color);
#else
	uint16_t* ptr = (uint16_t*)LCD_FRAME_BUFFER;
	uint32_t i = 0;

	while (i++ < LCD_WIDTH * LCD_HEIGHT) {
		*(ptr + i) = color;
	}
#endif
}

void lcd_init() {
	lcd_clear(WHITE);
	lcd_backlight_control(255);
}

void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color) {
	uint32_t pos;
	uint16_t* ptr;

	if (x > LCD_WIDTH || y > LCD_HEIGHT) {
		return;
	}

	// position offset
	pos = x + y * LCD_WIDTH;
	ptr = (uint16_t*)LCD_FRAME_BUFFER;

#if USE_DMA2D_EN
	dma2d_transfer_data_r2m((uint32_t*)(ptr + pos), 1, 1, 0, color);
#else
	*(ptr + pos) = color;
#endif
}

uint16_t lcd_read_point(uint16_t x, uint16_t y) {
	uint32_t pos;
	uint16_t *ptr, data;

	if (x > LCD_WIDTH || y > LCD_HEIGHT) {
		return 0;
	}

	// position offset
	pos = x + y * LCD_WIDTH;
	ptr = (uint16_t*)LCD_FRAME_BUFFER;

	data = *(ptr + pos);

	return data;
}

void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	uint16_t us;
	uint16_t usC_Current, usP_Current;

	int32_t lError_C = 0, lError_P = 0, lDelta_C, lDelta_P, lDistance;
	int32_t lIncrease_C, lIncrease_P;


	lDelta_C = x2 - x1;
	lDelta_P = y2 - y1;

	usC_Current = x1;
	usP_Current = y1;


	if (lDelta_C > 0)
		lIncrease_C = 1;

	else if (lDelta_C == 0)
		lIncrease_C = 0;

	else {
		lIncrease_C = -1;
		lDelta_C = -lDelta_C;
	}


	if (lDelta_P > 0)
		lIncrease_P = 1;

	else if (lDelta_P == 0)
		lIncrease_P = 0;
	else {
		lIncrease_P = -1;
		lDelta_P = -lDelta_P;
	}

	if (lDelta_C > lDelta_P)
		lDistance = lDelta_C;

	else
		lDistance = lDelta_P;


	for (us = 0; us <= lDistance + 1; us++) {
		lcd_draw_point(usC_Current, usP_Current, color);

		lError_C += lDelta_C;
		lError_P += lDelta_P;

		if (lError_C > lDistance) {
			lError_C -= lDistance;
			usC_Current += lIncrease_C;
		}

		if (lError_P > lDistance) {
			lError_P -= lDistance;
			usP_Current += lIncrease_P;
		}
	}
}