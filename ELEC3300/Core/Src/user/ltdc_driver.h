#ifndef _LCD_RGB_LTDC_DRV_H_
#define _LCD_RGB_LTDC_DRV_H_

#include "ltdc.h"

/**
 * @brief   Windows size on lcd.
 */
#define LCD_WIDTH  1024
#define LCD_HEIGHT 600

/**
 * @brief   Backlight control pin of lcd.
 */
#define LCD_BL_GPIO_PORT GPIOB
#define LCD_BL_GPIO_PIN	 GPIO_PIN_5

/**
 * @brief   start address of lcd framebuffer.
 */
#define LCD_FRAME_BUFFER 0xc0000000

/**
 * @brief   color
 * @note    rgb565
 */
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

/**
 * @brief       Control the lcd backlight.
 * @param[in]   brightness  the value of lcd backlight.
 * @return      None
 */
void lcd_backlight_control(uint8_t bightness);

/**
 * @brief       LCD initialization.
 * @param       None
 * @return      None
 */
void lcd_init(void);

/**
 * @brief       Clear lcd.
 * @param[in]   color   rgb565.
 * @return      None
 */
void lcd_clear(uint16_t color);

#endif /* _LCD_RGB_LTDC_DRV_H_ */
