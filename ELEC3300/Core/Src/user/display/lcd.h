#ifndef _LCD_H_
#define _LCD_H_

#include "ltdc.h"

#define LCD_WIDTH  1024
#define LCD_HEIGHT 600

// default spacing for font32
#define LCD_CHAR_SPACING_WIDTH	16
#define LCD_CHAR_SPACING_HEIGHT 24
#define LCD_MAX_CHAR_WIDTH		64
#define LCD_MAX_CHAR_HEIGHT		24

#define LCD_BL_GPIO_PORT GPIOB
#define LCD_BL_GPIO_PIN	 GPIO_PIN_5

#define LCD_FRAME_BUFFER 0xc0000000

#define BLACK  0x0000
#define BLUE   0x001F
#define GREEN  0x07E0
#define GBLUE  0X07FF
#define GRAY   0X8430
#define BROWN  0XBC40
#define RED	   0xF800
#define PINK   0XF81F
#define BRRED  0XFC07
#define YELLOW 0xFFE0
#define WHITE  0xFFFF

typedef uint16_t Color_t;

// @brief whether or not to use DMA for transfer
#define USE_DMA2D_EN 1

typedef enum { Font12, Font16, Font24, Font32 } FontSize_t;

void tft_backlight_control(uint8_t bightness);

void lcd_init(void);

void tft_clear(Color_t color);

void tft_draw_point(uint16_t x, uint16_t y, Color_t color);

uint16_t tft_read_point(uint16_t x, uint16_t y);

void tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color_t color);

void tft_fill_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color_t color);

void tft_print_char(uint16_t x, uint16_t y, char ch, Color_t color, FontSize_t fsize);

void tft_prints(uint8_t x, uint8_t y, const char* fmt, ...);

void tft_update();

void tft_set_text_color(Color_t color);

#endif /* _LCD_H_ */
