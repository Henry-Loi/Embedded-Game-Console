/**
 ****************************************************************************************************
 * @file        atk_rgblcd.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-24
 * @brief       ����ԭ��RGB LCDģ����������
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ F429������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * ������?:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __ATK_RGBLCD_H
#define __ATK_RGBLCD_H

// #include "./SYSTEM/sys/sys.h"
#include <stdint.h>

/* �����Ƿ�ʹ��DMA2D */
#define ATK_RGBLCD_USING_DMA2D 1

/* �����Ƿ�ʹ��RGB LCDģ�鴥�� */
#define ATK_RGBLCD_USING_TOUCH 1

/* ����RGB LCDģ�����õ����� */
#define ATK_RGBLCD_FONT_12 1
#define ATK_RGBLCD_FONT_16 1
#define ATK_RGBLCD_FONT_24 1
#define ATK_RGBLCD_FONT_32 1

/* Ĭ������DMA2D */
#ifndef ATK_RGBLCD_USING_DMA2D
	#define ATK_RGBLCD_USING_DMA2D 1
#endif

/* Ĭ�����ô��� */
#ifndef ATK_RGBLCD_USING_TOUCH
	#define ATK_RGBLCD_USING_TOUCH 1
#endif

/* Ĭ������12������ */
#if ((ATK_RGBLCD_FONT_12 == 0) && (ATK_RGBLCD_FONT_16 == 0) && (ATK_RGBLCD_FONT_24 == 0) && (ATK_RGBLCD_FONT_32 == 0))
	#undef ATK_RGBLCD_FONT_12
	#define ATK_RGBLCD_FONT_12 1
#endif

/* RGB LCDģ��ʱ��ṹ��? */
typedef struct {
	uint32_t clock_freq;
	uint8_t pixel_clock_polarity;
	uint16_t hactive;
	uint16_t hback_porch;
	uint16_t hfront_porch;
	uint16_t hsync_len;
	uint16_t vactive;
	uint16_t vback_porch;
	uint16_t vfront_porch;
	uint16_t vsync_len;
} atk_rgblcd_timing_t;

/* RGB LCD����IC����ö�� */
typedef enum {
	ATK_RGBLCD_TOUCH_TYPE_GTXX = 0x00, /* GT9111��GT9147��GT9271 */
	ATK_RGBLCD_TOUCH_TYPE_FTXX,		   /* FT5206 */
} atk_rgblcd_touch_type_t;

/* ����ͷ�ļ� */
#if (ATK_RGBLCD_USING_TOUCH != 0)
	#include "atk_rgblcd_touch.h"
#endif

/* ���Ŷ��� */
#define ATK_RGBLCD_BL_GPIO_PORT GPIOB
#define ATK_RGBLCD_BL_GPIO_PIN	GPIO_PIN_5
#define ATK_RGBLCD_BL_GPIO_CLK_ENABLE() \
	do {                                \
		__HAL_RCC_GPIOB_CLK_ENABLE();   \
	} while (0)
#define ATK_RGBLCD_M0_GPIO_PORT GPIOG
#define ATK_RGBLCD_M0_GPIO_PIN	GPIO_PIN_6
#define ATK_RGBLCD_M0_GPIO_CLK_ENABLE() \
	do {                                \
		__HAL_RCC_GPIOG_CLK_ENABLE();   \
	} while (0)
#define ATK_RGBLCD_M1_GPIO_PORT GPIOI
#define ATK_RGBLCD_M1_GPIO_PIN	GPIO_PIN_2
#define ATK_RGBLCD_M1_GPIO_CLK_ENABLE() \
	do {                                \
		__HAL_RCC_GPIOI_CLK_ENABLE();   \
	} while (0)
#define ATK_RGBLCD_M2_GPIO_PORT GPIOI
#define ATK_RGBLCD_M2_GPIO_PIN	GPIO_PIN_7
#define ATK_RGBLCD_M2_GPIO_CLK_ENABLE() \
	do {                                \
		__HAL_RCC_GPIOI_CLK_ENABLE();   \
	} while (0)

/* IO���� */
#define ATK_RGBLCD_BL(x)                                                                        \
	do {                                                                                        \
		x ? HAL_GPIO_WritePin(ATK_RGBLCD_BL_GPIO_PORT, ATK_RGBLCD_BL_GPIO_PIN, GPIO_PIN_SET)    \
		  : HAL_GPIO_WritePin(ATK_RGBLCD_BL_GPIO_PORT, ATK_RGBLCD_BL_GPIO_PIN, GPIO_PIN_RESET); \
	} while (0)
#define ATK_RGBLCD_READ_M0() HAL_GPIO_ReadPin(ATK_RGBLCD_M0_GPIO_PORT, ATK_RGBLCD_M0_GPIO_PIN)
#define ATK_RGBLCD_READ_M1() HAL_GPIO_ReadPin(ATK_RGBLCD_M1_GPIO_PORT, ATK_RGBLCD_M1_GPIO_PIN)
#define ATK_RGBLCD_READ_M2() HAL_GPIO_ReadPin(ATK_RGBLCD_M2_GPIO_PORT, ATK_RGBLCD_M2_GPIO_PIN)

/* RGB LCDģ��PIDö�� */
enum {
	ATK_RGBLCD_PID_4342 = 0x4342, /* ATK-MD0430R-480272 */
	ATK_RGBLCD_PID_7084 = 0x7084, /* ATK-MD0700R-800480 */
	ATK_RGBLCD_PID_7016 = 0x7016, /* ATK-MD0700R-1024600 */
	ATK_RGBLCD_PID_7018 = 0x7018, /* ATK-MD0700R-1280800 */
	ATK_RGBLCD_PID_4384 = 0x4384, /* ATK-MD0430R-800480 */
	ATK_RGBLCD_PID_1018 = 0x1018, /* ATK-MD1010R-1280800 */
};

/* RGB LCDģ��LCD��ת����ö�� */
typedef enum {
	ATK_RGBLCD_LCD_DISP_DIR_0 = 0x00, /* LCD˳ʱ����ת0����ʾ���� */
	ATK_RGBLCD_LCD_DISP_DIR_90,		  /* LCD˳ʱ����ת90����ʾ���� */
	ATK_RGBLCD_LCD_DISP_DIR_180,	  /* LCD˳ʱ����ת180����ʾ���� */
	ATK_RGBLCD_LCD_DISP_DIR_270,	  /* LCD˳ʱ����ת270����ʾ���� */
} atk_rgblcd_lcd_disp_dir_t;

/* RGB LCDģ��LCD��ʾ����ö�� */
typedef enum {
#if (ATK_RGBLCD_FONT_12 != 0)
	ATK_RGBLCD_LCD_FONT_12, /* 12������ */
#endif
#if (ATK_RGBLCD_FONT_16 != 0)
	ATK_RGBLCD_LCD_FONT_16, /* 16������ */
#endif
#if (ATK_RGBLCD_FONT_24 != 0)
	ATK_RGBLCD_LCD_FONT_24, /* 24������ */
#endif
#if (ATK_RGBLCD_FONT_32 != 0)
	ATK_RGBLCD_LCD_FONT_32, /* 32������ */
#endif
} atk_rgblcd_lcd_font_t;

/* RGB LCDģ��LCD��ʾ����ģʽö�� */
typedef enum {
	ATK_RGBLCD_NUM_SHOW_NOZERO = 0x00, /* ���ָ�λ0����ʾ */
	ATK_RGBLCD_NUM_SHOW_ZERO,		   /* ���ָ�λ0��ʾ */
} atk_rgblcd_num_mode_t;

/* ������ɫ���� */
#define ATK_RGBLCD_WHITE   0xFFFF
#define ATK_RGBLCD_BLACK   0x0000
#define ATK_RGBLCD_BLUE	   0x001F
#define ATK_RGBLCD_BRED	   0xF81F
#define ATK_RGBLCD_GRED	   0xFFE0
#define ATK_RGBLCD_GBLUE   0x07FF
#define ATK_RGBLCD_RED	   0xF800
#define ATK_RGBLCD_MAGENTA 0xF81F
#define ATK_RGBLCD_GREEN   0x07E0
#define ATK_RGBLCD_CYAN	   0x7FFF
#define ATK_RGBLCD_YELLOW  0xFFE0
#define ATK_RGBLCD_BROWN   0xBC40
#define ATK_RGBLCD_BRRED   0xFC07
#define ATK_RGBLCD_GRAY	   0x8430

/* �������? */
#define ATK_RGBLCD_EOK	  0 /* û�д��� */
#define ATK_RGBLCD_ERROR  1 /* ���� */
#define ATK_RGBLCD_EINVAL 2 /* �Ƿ����� */

/* �������� */
uint8_t atk_rgblcd_init(void);			  /* RGB LCDģ���ʼ��? */
uint16_t atk_rgblcd_get_pid(void);		  /* ��ȡRGB LCDģ��PID */
uint16_t atk_rgblcd_get_lcd_width(void);  /* ��ȡRGB LCDģ��LCD���� */
uint16_t atk_rgblcd_get_lcd_height(void); /* ��ȡRGB LCDģ��LCD�߶� */
void atk_rgblcd_backlight_on(void);		  /* ����RGB LCDģ��LCD���� */
void atk_rgblcd_backlight_off(void);	  /* �ر�RGB LCDģ��LCD���� */
void atk_rgblcd_display_on(void);		  /* ����RGB LCDģ��LCD��ʾ */
void atk_rgblcd_display_off(void);		  /* �ر�RGB LCDģ��LCD��ʾ */
uint8_t atk_rgblcd_set_disp_dir(
	atk_rgblcd_lcd_disp_dir_t disp_dir); /* ����RGB LCDģ��LCD��ʾ���� */
atk_rgblcd_lcd_disp_dir_t atk_rgblcd_get_disp_dir(void); /* ��ȡRGB LCDģ��LCDɨ�跽�� */
void atk_rgblcd_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye,
					 uint16_t color);  /* RGB LCDģ��LCD�������? */
void atk_rgblcd_clear(uint16_t color); /* RGB LCDģ��LCD���� */
void atk_rgblcd_draw_point(uint16_t x, uint16_t y, uint16_t color); /* RGB LCDģ��LCD���� */
uint16_t atk_rgblcd_read_point(uint16_t x, uint16_t y);				/* RGB LCDģ��LCD���� */
void atk_rgblcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
						  uint16_t color); /* RGB LCDģ��LCD���߶� */
void atk_rgblcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
						  uint16_t color); /* RGB LCDģ��LCD�����ο� */
void atk_rgblcd_draw_circle(uint16_t x, uint16_t y, uint16_t r,
							uint16_t color); /* RGB LCDģ��LCD��Բ�ο� */
void atk_rgblcd_show_char(uint16_t x, uint16_t y, char ch, atk_rgblcd_lcd_font_t font,
						  uint16_t color); /* RGB LCDģ��LCD��ʾ1���ַ� */
void atk_rgblcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char* str,
							atk_rgblcd_lcd_font_t font, uint16_t color); /* RGB LCDģ��LCD��ʾ�ַ��� */
void atk_rgblcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_rgblcd_num_mode_t mode,
						  atk_rgblcd_lcd_font_t font,
						  uint16_t color); /* RGB LCDģ��LCD��ʾ���֣��ɿ�����ʾ��λ0 */
void atk_rgblcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_rgblcd_lcd_font_t font,
						 uint16_t color); /* RGB LCDģ��LCD��ʾ���֣�����ʾ��λ0 */

#endif
