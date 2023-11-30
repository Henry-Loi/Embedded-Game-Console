#include "lcd.h"

#include "dma2d.h"
#include "lcd_font.h"
#include "strhelper.h"
#include "tim.h"

#include "stm32f429xx.h"

typedef struct {
	uint8_t curr_buffer;
	char char_buffer[2][LCD_MAX_CHAR_WIDTH][LCD_MAX_CHAR_HEIGHT];
	Color_t color_buffer[2][LCD_MAX_CHAR_WIDTH][LCD_MAX_CHAR_HEIGHT];
	FontSize_t fontsize_buffer[2][LCD_MAX_CHAR_WIDTH][LCD_MAX_CHAR_HEIGHT];
	Color_t text_color;
} TFTBuffer_t;

TFTBuffer_t tft_buffer = {.color_buffer = BLACK};

#define CURR_BUF  tft_buffer.curr_buffer
#define CHAR_BUF  tft_buffer.char_buffer
#define COLOR_BUF tft_buffer.color_buffer
#define FSIZE_BUF tft_buffer.fontsize_buffer

/* void tft_backlight_control(uint8_t brightness) {
	if (brightness) {
		HAL_GPIO_WritePin(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN, GPIO_PIN_RESET);
	}
} */
void tft_backlight_control(uint8_t brightness) { TIM3->CCR2 = brightness; }

void tft_clear(Color_t color) {
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

void tft_clear_buf(uint8_t buf) {
	for (int x = 0; x < LCD_MAX_CHAR_WIDTH; x++) {
		for (int y = 0; y < LCD_MAX_CHAR_HEIGHT; y++) {
			CHAR_BUF[buf][x][y] = ' ';
		}
	}
}

void lcd_init() {
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	TIM3->ARR = 255;
	TIM3->CCR2 = 255;

	tft_clear(WHITE);
	tft_clear_buf(0);
	tft_clear_buf(1);
	tft_backlight_control(255);

	for (int x = 0; x < LCD_MAX_CHAR_WIDTH; x++) {
		for (int y = 0; y < LCD_MAX_CHAR_HEIGHT; y++) {
			COLOR_BUF[CURR_BUF][x][y] = WHITE;
			FSIZE_BUF[CURR_BUF][x][y] = Font32;
			COLOR_BUF[!CURR_BUF][x][y] = WHITE;
			FSIZE_BUF[!CURR_BUF][x][y] = Font32;
		}
	}
}


void tft_draw_point(uint16_t x, uint16_t y, Color_t color) {
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

uint16_t tft_read_point(uint16_t x, uint16_t y) {
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

void tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color_t color) {
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
		tft_draw_point(usC_Current, usP_Current, color);

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

void tft_fill_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color_t color) {
	for (int i = y1; i < y2; i++) {
		tft_draw_line(x1, i, x2, i, color);
	}
}

// void tft_print_char(uint16_t x, uint16_t y, char ch, Color_t color, FontSize_t fsize) {
void tft_write_char(uint16_t x, uint16_t y, char ch, Color_t color, FontSize_t fsize) {
	const uint8_t* ch_code;
	uint8_t ch_width;
	uint8_t ch_height;
	uint8_t ch_size;
	uint8_t ch_offset;
	uint8_t byte_index;
	uint8_t byte_code;
	uint8_t bit_index;
	uint8_t width_index = 0;
	uint8_t height_index = 0;

	uint16_t px = x * LCD_CHAR_SPACING_WIDTH;
	uint16_t py = y * LCD_CHAR_SPACING_HEIGHT;

	ch_offset = ch - ' ';

	switch (fsize) {
		case Font12:
			ch_code = rgblcd_font_12[ch_offset];
			ch_width = LCD_FONT_12_CHAR_WIDTH;
			ch_height = LCD_FONT_12_CHAR_HEIGHT;
			ch_size = LCD_FONT_12_CHAR_SIZE;
			break;

		case Font16:
			ch_code = rgblcd_font_16[ch_offset];
			ch_width = LCD_FONT_16_CHAR_WIDTH;
			ch_height = LCD_FONT_16_CHAR_HEIGHT;
			ch_size = LCD_FONT_16_CHAR_SIZE;
			break;
		case Font24:
			ch_code = rgblcd_font_24[ch_offset];
			ch_width = LCD_FONT_24_CHAR_WIDTH;
			ch_height = LCD_FONT_24_CHAR_HEIGHT;
			ch_size = LCD_FONT_24_CHAR_SIZE;
			break;
		case Font32:
			ch_code = rgblcd_font_32[ch_offset];
			ch_width = LCD_FONT_32_CHAR_WIDTH;
			ch_height = LCD_FONT_32_CHAR_HEIGHT;
			ch_size = LCD_FONT_32_CHAR_SIZE;
			break;
		default: break;
	}


	if ((px + ch_width > LCD_WIDTH) || (py + ch_height > LCD_HEIGHT)) {
		return;
	}

	for (byte_index = 0; byte_index < ch_size; byte_index++) {
		byte_code = ch_code[byte_index];
		for (bit_index = 0; bit_index < 8; bit_index++) {
			if ((byte_code & 0x80) != 0) {
				tft_draw_point(px + width_index, py + height_index, color);
			}
			width_index++;
			if (width_index == ch_width) {
				width_index = 0;
				height_index++;
				break;
			}
			byte_code <<= 1;
		}
	}
}

void tft_print_char(uint16_t x, uint16_t y, char ch, Color_t color, FontSize_t fsize) {
	CHAR_BUF[CURR_BUF][x][y] = ch;
	COLOR_BUF[CURR_BUF][x][y] = color;
	FSIZE_BUF[CURR_BUF][x][y] = fsize;
}

void tft_prints(uint8_t x, uint8_t y, const char* fmt, ...) {
	char buf[255] = {0}, *fp = buf;

	va_list args;
	va_start(args, fmt);
	print(&fp, fmt, args);

	// TODO maybe don't hardcode text color
	for (int i = 0; buf[i] != NULL; i++)
		tft_print_char(x + i, y, buf[i], tft_buffer.text_color, Font32);
}

void tft_set_text_color(Color_t color) { tft_buffer.text_color = color; }

// NOTE: THIS ONLY WORKS FOR CHARS NOW BECAUSE I CAN'T THINK OF A EASY WAY TO DEAL WITH THE ENTIRE DISPLAY
// MAKING A 1024x600 BUFFER IS WAY TOO LARGE IT CAN'T FIT
// This means that updating doesn't work for draw dot/line
void tft_update() {
	for (int x = 0; x < LCD_MAX_CHAR_WIDTH; x++) {
		for (int y = 0; y < LCD_MAX_CHAR_HEIGHT; y++) {
			if (CHAR_BUF[CURR_BUF][x][y] != CHAR_BUF[!CURR_BUF][x][y]) {
				// TODO maybe don't hardcode bg color
				tft_write_char(x, y, CHAR_BUF[!CURR_BUF][x][y], WHITE, FSIZE_BUF[CURR_BUF][x][y]);
				tft_write_char(x, y, CHAR_BUF[CURR_BUF][x][y], COLOR_BUF[CURR_BUF][x][y], FSIZE_BUF[CURR_BUF][x][y]);
			}
		}
	}
	CURR_BUF = !CURR_BUF;
	tft_clear_buf(CURR_BUF);
}
