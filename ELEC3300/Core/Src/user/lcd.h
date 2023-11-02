#ifndef _LCD_H_
#define _LCD_H_

#include "ltdc.h"

#define LCD_WIDTH  1024
#define LCD_HEIGHT 600

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

// @brief whether or not to use DMA for transfer
#define USE_DMA2D_EN 1

void lcd_backlight_control(uint8_t bightness);

void lcd_init(void);

void lcd_clear(uint16_t color);

void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);

uint16_t lcd_read_point(uint16_t x, uint16_t y);

void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

#endif /* _LCD_H_ */
