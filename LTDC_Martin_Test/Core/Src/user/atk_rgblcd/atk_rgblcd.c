/**
 ****************************************************************************************************
 * @file        atk_rgblcd.c
 * @author      ??????????(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-24
 * @brief       ???????RGB LCD???????????
 * @license     Copyright (c) 2020-2032,
 *??????????????????????
 ****************************************************************************************************
 * @attention
 *
 * ?????:??????? ?????? F429??????
 * ???????:www.yuanzige.com
 * ???????:www.openedv.com
 * ??????:www.alientek.com
 * ??????:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "atk_rgblcd.h"

// #include "./SYSTEM/usart/usart.h"
#include "../board.h"
#include "atk_rgblcd_font.h"
#include "atk_rgblcd_ltdc.h"
#include "gpio.h"


#define ATK_RGBLCD_LCD_RAW_WIDTH  g_atk_rgblcd_sta.param->width
#define ATK_RGBLCD_LCD_RAW_HEIGHT g_atk_rgblcd_sta.param->height
#define ATK_RGBLCD_LCD_WIDTH	  g_atk_rgblcd_sta.width
#define ATK_RGBLCD_LCD_HEIGHT	  g_atk_rgblcd_sta.height
#define ATK_RGBLCD_FB			  g_atk_rgblcd_sta.fb

/* RGB LCD?????????? */

/* RGB LCD?????????? */
static const atk_rgblcd_param_t atk_rgblcd_param[] = {
	{0,
	 ATK_RGBLCD_PID_4342,
	 480,
	 272,
	 {9000000, 1, 480, 40, 5, 1, 272, 8, 8, 1},
	 ATK_RGBLCD_TOUCH_TYPE_GTXX}, // ATK-MD0430R-480272
	{1,
	 ATK_RGBLCD_PID_7084,
	 800,
	 480,
	 {33000000, 1, 800, 46, 210, 1, 480, 23, 22, 1},
	 ATK_RGBLCD_TOUCH_TYPE_FTXX}, // ATK-MD0700R-800480
	{2,
	 ATK_RGBLCD_PID_7016,
	 1024,
	 600,
	 {45000000, 1, 1024, 140, 160, 20, 600, 20, 12, 3},
	 ATK_RGBLCD_TOUCH_TYPE_GTXX}, // ATK-MD0700R-1024600
	{3,
	 ATK_RGBLCD_PID_7018,
	 1280,
	 800,
	 {0, 1, 1280, 0, 0, 0, 800, 0, 0, 0},
	 ATK_RGBLCD_TOUCH_TYPE_GTXX}, // ATK-MD0700R-1280800
	{4,
	 ATK_RGBLCD_PID_4384,
	 800,
	 480,
	 {33000000, 1, 800, 88, 40, 48, 480, 32, 13, 3},
	 ATK_RGBLCD_TOUCH_TYPE_GTXX}, // ATK-MD0430R-800480
	{5,
	 ATK_RGBLCD_PID_1018,
	 1280,
	 800,
	 {45000000, 0, 1280, 140, 10, 10, 800, 10, 10, 3},
	 ATK_RGBLCD_TOUCH_TYPE_GTXX}, // ATK-MD1010R-1280800
};

/* RGB LCD???????????? */
RGBLCD_Status_t g_atk_rgblcd_sta = {0};

/**
 * @brief       RGB LCD???ID???
 * @param       ??
 * @retval      RGB LCD???ID
 */
static uint8_t atk_rgblcd_get_id(void) {
	GPIO_InitTypeDef gpio_init_struct = {0};
	uint8_t id;

	ATK_RGBLCD_M0_GPIO_CLK_ENABLE();
	ATK_RGBLCD_M1_GPIO_CLK_ENABLE();
	ATK_RGBLCD_M2_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = ATK_RGBLCD_M0_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;
	gpio_init_struct.Pull = GPIO_NOPULL;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ATK_RGBLCD_M0_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = ATK_RGBLCD_M1_GPIO_PIN;
	HAL_GPIO_Init(ATK_RGBLCD_M1_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = ATK_RGBLCD_M2_GPIO_PIN;
	HAL_GPIO_Init(ATK_RGBLCD_M2_GPIO_PORT, &gpio_init_struct);

	id = ATK_RGBLCD_READ_M0();
	id |= ATK_RGBLCD_READ_M1() << 1;
	id |= ATK_RGBLCD_READ_M2() << 2;

	return id;
}

/**
 * @brief       RGB LCD???????????
 * @param       ???RGB LCD??ID???RGB LCD????????
 * @retval      ATK_RGBLCD_EOK   : RGB LCD??????????????
 *              ATK_RGBLCD_EINVAL: ????ID????
 */
static uint8_t atk_rgblcd_setup_param_by_id(uint8_t id) {
	uint8_t index;

	for (index = 0; index < (sizeof(atk_rgblcd_param) / sizeof(atk_rgblcd_param[0])); index++) {
		if (id == atk_rgblcd_param[index].id) {
			g_atk_rgblcd_sta.param = &atk_rgblcd_param[index];
			return ATK_RGBLCD_EOK;
		}
	}

	return ATK_RGBLCD_EINVAL;
}

/**
 * @brief       RGB LCD???????????
 * @param       ??
 * @retval      ATK_RGBLCD_EOK  : RGB LCD??????????
 *              ATK_RGBLCD_ERROR: RGB LCD??????????
 */
static void atk_rgblcd_hw_init(void) {
	GPIO_InitTypeDef gpio_init_struct = {0};

	ATK_RGBLCD_BL_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = ATK_RGBLCD_BL_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Pull = GPIO_PULLUP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ATK_RGBLCD_BL_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief       ?????????x^y
 * @param       x: ????
 *              y: ???
 * @retval      x^y
 */
static uint32_t atk_rgblcd_pow(uint8_t x, uint8_t y) {
	uint8_t loop;
	uint32_t res = 1;

	for (loop = 0; loop < y; loop++) {
		res *= x;
	}

	return res;
}

/**
 * @brief       ????RGB LCD?????????????????
 * @param       x: X?????
 *              y: Y?????
 * @retval      ??
 */
static inline void atk_rgblcd_pos_transform(uint16_t* x, uint16_t* y) {
	uint16_t x_target;
	uint16_t y_target;

	switch (g_atk_rgblcd_sta.disp_dir) {
		case ATK_RGBLCD_LCD_DISP_DIR_0: {
			x_target = *x;
			y_target = *y;
			break;
		}
		case ATK_RGBLCD_LCD_DISP_DIR_90: {
			x_target = ATK_RGBLCD_LCD_RAW_WIDTH - *y - 1;
			y_target = *x;
			break;
		}
		case ATK_RGBLCD_LCD_DISP_DIR_180: {
			x_target = ATK_RGBLCD_LCD_RAW_WIDTH - *x - 1;
			y_target = ATK_RGBLCD_LCD_RAW_HEIGHT - *y - 1;
			break;
		}
		case ATK_RGBLCD_LCD_DISP_DIR_270: {
			x_target = *y;
			y_target = ATK_RGBLCD_LCD_RAW_HEIGHT - *x - 1;
			break;
		}
	}

	*x = x_target;
	*y = y_target;
}

#if (ATK_RGBLCD_USING_DMA2D != 0)
/**
 * @brief       DMA2D?????
 * @param       ??
 * @retval      ??
 */
static void atk_rgblcd_dma2d_init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN; /* ???DMA2D??? */
	DMA2D->CR &= ~DMA2D_CR_START;		 /* ??DMA2D */
	DMA2D->CR &= ~DMA2D_CR_MODE_Msk;	 /* ?????????????? */
	DMA2D->CR |= DMA2D_R2M;
	DMA2D->OPFCCR &= ~DMA2D_OPFCCR_CM_Msk; /* RGB565?? */
	DMA2D->OPFCCR |= DMA2D_OUTPUT_RGB565;
}

/**
 * @brief       DMA2D LCD???????
 * @param       xs   : ???????X????
 *              ys   : ???????Y????
 *              xe   : ???????X????
 *              ye   : ???????Y????
 *              color: ??????????
 * @retval      ??
 */
static inline void atk_rgblcd_dma2d_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color) {
	uint32_t timeout = 0;

	atk_rgblcd_pos_transform(&xs, &ys);
	atk_rgblcd_pos_transform(&xe, &ye);


	if (xs > xe) {
		xs = xs ^ xe;
		xe = xs ^ xe;
		xs = xs ^ xe;
	}
	if (ys > ye) {
		ys = ys ^ ye;
		ye = ys ^ ye;
		ys = ys ^ ye;
	}

	DMA2D->CR &= ~DMA2D_CR_START;												/* ??DMA2D */
	DMA2D->OOR = ATK_RGBLCD_LCD_RAW_WIDTH - (xe - xs + 1);						/* ????? */
	DMA2D->OMAR = (uint32_t)&ATK_RGBLCD_FB[ys * ATK_RGBLCD_LCD_RAW_WIDTH + xs]; /* ???????? */

	DMA2D->NLR &= ~DMA2D_NLR_PL_Msk; /* ??????????? */
	DMA2D->NLR |= ((xe - xs + 1) << DMA2D_NLR_PL_Pos);
	DMA2D->NLR &= ~DMA2D_NLR_NL_Msk; /* ??????? */
	DMA2D->NLR |= ((ye - ys + 1) << DMA2D_NLR_NL_Pos);
	DMA2D->OCOLR = color;		 /* ?????? */
	DMA2D->CR |= DMA2D_CR_START; /* ????DMA2D */
								 // STUCK HERE

	while ((DMA2D->ISR & DMA2D_ISR_TCIF) == 0) {
		// timeout++;
		// if (timeout > 0X1FFFFF)
		// 	break;
	}
	led_off(LED1);


	DMA2D->IFCR |= DMA2D_IFCR_CTCIF; /* ???????????? */
}
#endif

/**
 * @brief       RGB LCD???????
 * @param       ??
 * @retval      ATK_RGBLCD_EOK  : RGB LCD??????????
 *              ATK_RGBLCD_ERROR: RGB LCD??????????
 */
uint8_t atk_rgblcd_init(void) {
	uint8_t id;
	uint8_t ret;

	id = atk_rgblcd_get_id();				/* RGB LCD???ID??? */
	ret = atk_rgblcd_setup_param_by_id(id); /* RGB LCD??????????? */
	if (ret != ATK_RGBLCD_EOK) {
		return ATK_RGBLCD_ERROR;
	}

	atk_rgblcd_hw_init(); /* RGB LCD??????????? */
	atk_rgblcd_ltdc_init(ATK_RGBLCD_LCD_RAW_WIDTH, ATK_RGBLCD_LCD_RAW_HEIGHT,
						 &g_atk_rgblcd_sta.param->timing); /* RGB LCD???LTDC??????? */
	g_atk_rgblcd_sta.fb = (uint16_t*)ATK_RGBLCD_LTDC_LAYER_FB_ADDR;
	// #if (ATK_RGBLCD_USING_DMA2D != 0)
	//
	atk_rgblcd_dma2d_init(); /* ?????DMA2D */
							 // #endif
	atk_rgblcd_set_disp_dir(ATK_RGBLCD_LCD_DISP_DIR_0);

	atk_rgblcd_clear(ATK_RGBLCD_WHITE);

	atk_rgblcd_backlight_on(); /* ????RGB LCD???LCD???? */

#if (ATK_RGBLCD_USING_TOUCH != 0)
	ret = atk_rgblcd_touch_init(g_atk_rgblcd_sta.param->touch_type);
	if (ret != ATK_RGBLCD_TOUCH_EOK) {
		return ATK_RGBLCD_ERROR;
	}
#endif

	return ATK_RGBLCD_EOK;
}

/**
 * @brief       ???RGB LCD???PID
 * @param       ??
 * @retval      0     : RGB LCD??????????
 *              ?????: RGB LCD???PID
 */
uint16_t atk_rgblcd_get_pid(void) { return g_atk_rgblcd_sta.param->pid; }

/**
 * @brief       ???RGB LCD???LCD????
 * @param       ??
 * @retval      0     : RGB LCD??????????
 *              ?????: RGB LCD???LCD????
 */
uint16_t atk_rgblcd_get_lcd_width(void) { return g_atk_rgblcd_sta.width; }

/**
 * @brief       ???RGB LCD???LCD???
 * @param       ??
 * @retval      0     : RGB LCD??????????
 *              ?????: RGB LCD???LCD???
 */
uint16_t atk_rgblcd_get_lcd_height(void) { return g_atk_rgblcd_sta.height; }

/**
 * @brief       ????RGB LCD???LCD????
 * @param       ??
 * @retval      ??
 */
void atk_rgblcd_backlight_on(void) { ATK_RGBLCD_BL(1); }

/**
 * @brief       ???RGB LCD???LCD????
 * @param       ??
 * @retval      ??
 */
void atk_rgblcd_backlight_off(void) { ATK_RGBLCD_BL(0); }

/**
 * @brief       ????RGB LCD???LCD???
 * @param       ??
 * @retval      ??
 */
void atk_rgblcd_display_on(void) { atk_rgblcd_ltdc_enable(); }

/**
 * @brief       ???RGB LCD???LCD???
 * @param       ??
 * @retval      ??
 */
void atk_rgblcd_display_off(void) { atk_rgblcd_ltdc_disable(); }

/**
 * @brief       ????RGB LCD???LCD???????
 * @param       disp_dir: ATK_RGBLCD_LCD_DISP_DIR_0  : LCD???????0?????????
 *                        ATK_RGBLCD_LCD_DISP_DIR_90 : LCD???????90?????????
 *                        ATK_RGBLCD_LCD_DISP_DIR_180: LCD???????180?????????
 *                        ATK_RGBLCD_LCD_DISP_DIR_270: LCD???????270?????????
 * @retval      ATK_RGBLCD_EOK   : ????RGB LCD???LCD?????????
 *              ATK_RGBLCD_EINVAL: ???????????
 */
uint8_t atk_rgblcd_set_disp_dir(atk_rgblcd_lcd_disp_dir_t disp_dir) {
	switch (disp_dir) {
		case ATK_RGBLCD_LCD_DISP_DIR_0:
		case ATK_RGBLCD_LCD_DISP_DIR_180: {
			g_atk_rgblcd_sta.width = g_atk_rgblcd_sta.param->width;
			g_atk_rgblcd_sta.height = g_atk_rgblcd_sta.param->height;
			break;
		}
		case ATK_RGBLCD_LCD_DISP_DIR_90:
		case ATK_RGBLCD_LCD_DISP_DIR_270: {
			g_atk_rgblcd_sta.width = g_atk_rgblcd_sta.param->height;
			g_atk_rgblcd_sta.height = g_atk_rgblcd_sta.param->width;
			break;
		}
		default: {
			return ATK_RGBLCD_EINVAL;
		}
	}

	g_atk_rgblcd_sta.disp_dir = disp_dir;

	return ATK_RGBLCD_EOK;
}

/**
 * @brief       ???RGB LCD???LCD??????
 * @param       ??
 * @retval      RGB LCD???LCD??????
 */
atk_rgblcd_lcd_disp_dir_t atk_rgblcd_get_disp_dir(void) { return g_atk_rgblcd_sta.disp_dir; }

/**
 * @brief       RGB LCD???LCD???????
 * @param       xs   : ???????X????
 *              ys   : ???????Y????
 *              xe   : ???????X????
 *              ye   : ???????Y????
 *              color: ??????????
 * @retval      ??
 */
void atk_rgblcd_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color) {
	if (xe >= ATK_RGBLCD_LCD_WIDTH) {
		xe = ATK_RGBLCD_LCD_WIDTH - 1;
	}

	if (ye >= ATK_RGBLCD_LCD_HEIGHT) {
		ye = ATK_RGBLCD_LCD_HEIGHT - 1;
	}


#if (ATK_RGBLCD_USING_DMA2D != 0)
	atk_rgblcd_dma2d_fill(xs, ys, xe, ye, color);
#else
	uint16_t x_index;
	uint16_t y_index;

	for (y_index = ys; y_index < ye + 1; y_index++) {
		for (x_index = xs; x_index < xe + 1; x_index++) {
			atk_rgblcd_pos_transform(&x_index, &y_index);
			ATK_RGBLCD_FB[y_index * ATK_RGBLCD_LCD_RAW_WIDTH + x_index] = color;
		}
	}
#endif
}

/**
 * @brief       RGB LCD???LCD????
 * @param       color: ???????
 * @retval      ??
 */
void atk_rgblcd_clear(uint16_t color) {
	atk_rgblcd_fill(0, 0, ATK_RGBLCD_LCD_WIDTH - 1, ATK_RGBLCD_LCD_HEIGHT - 1, color);
}

/**
 * @brief       RGB LCD???LCD????
 * @param       x    : ???????X????
 *              y    : ???????Y????
 *              color: ??????????
 * @retval      ??
 */
void atk_rgblcd_draw_point(uint16_t x, uint16_t y, uint16_t color) {
	atk_rgblcd_pos_transform(&x, &y);
	ATK_RGBLCD_FB[y * ATK_RGBLCD_LCD_RAW_WIDTH + x] = color;
}

/**
 * @brief       RGB LCD???LCD????
 * @param       x    : ???????X????
 *              y    : ???????Y????
 * @retval      ??????????
 */
uint16_t atk_rgblcd_read_point(uint16_t x, uint16_t y) {
	atk_rgblcd_pos_transform(&x, &y);
	return ATK_RGBLCD_FB[y * ATK_RGBLCD_LCD_RAW_WIDTH + x];
}

/**
 * @brief       RGB LCD???LCD?????
 * @param       x1   : ??????????1??X????
 *              y1   : ??????????1??Y????
 *              x2   : ??????????2??X????
 *              y2   : ??????????2??Y????
 *              color: ????????????
 * @retval      ??
 */
void atk_rgblcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	uint16_t x_delta;
	uint16_t y_delta;
	int16_t x_sign;
	int16_t y_sign;
	int16_t error;
	int16_t error2;

	x_delta = (x1 < x2) ? (x2 - x1) : (x1 - x2);
	y_delta = (y1 < y2) ? (y2 - y1) : (y1 - y2);
	x_sign = (x1 < x2) ? 1 : -1;
	y_sign = (y1 < y2) ? 1 : -1;
	error = x_delta - y_delta;

	atk_rgblcd_draw_point(x2, y2, color);

	while ((x1 != x2) || (y1 != y2)) {
		atk_rgblcd_draw_point(x1, y1, color);

		error2 = error << 1;
		if (error2 > -y_delta) {
			error -= y_delta;
			x1 += x_sign;
		}

		if (error2 < x_delta) {
			error += x_delta;
			y1 += y_sign;
		}
	}
}

/**
 * @brief       RGB LCD???LCD????????
 * @param       x1   : ????????????1??X????
 *              y1   : ????????????1??Y????
 *              x2   : ????????????2??X????
 *              y2   : ????????????2??Y????
 *              color: ??????????????
 * @retval      ??
 */
void atk_rgblcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	atk_rgblcd_draw_line(x1, y1, x2, y1, color);
	atk_rgblcd_draw_line(x1, y2, x2, y2, color);
	atk_rgblcd_draw_line(x1, y1, x1, y2, color);
	atk_rgblcd_draw_line(x2, y1, x2, y2, color);
}

/**
 * @brief       RGB LCD???LCD???????
 * @param       x    : ?????????????X????
 *              y    : ?????????????Y????
 *              r    : ???????????
 *              color: ?????????????
 * @retval      ??
 */
void atk_rgblcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
	int32_t x_t;
	int32_t y_t;
	int32_t error;
	int32_t error2;

	x_t = -r;
	y_t = 0;
	error = 2 - 2 * r;

	do {
		atk_rgblcd_draw_point(x - x_t, y + y_t, color);
		atk_rgblcd_draw_point(x + x_t, y + y_t, color);
		atk_rgblcd_draw_point(x + x_t, y - y_t, color);
		atk_rgblcd_draw_point(x - x_t, y - y_t, color);

		error2 = error;
		if (error2 <= y_t) {
			y_t++;
			error = error + (y_t * 2 + 1);
			if ((-x_t == y_t) && (error2 <= x_t)) {
				error2 = 0;
			}
		}

		if (error2 > x_t) {
			x_t++;
			error = error + (x_t * 2 + 1);
		}
	} while (x_t <= 0);
}

/**
 * @brief       RGB LCD???LCD???1?????
 * @param       x    : ??????????X????
 *              y    : ??????????Y????
 *              ch   : ????????
 *              font : ??????????????
 *              color: ?????????????
 * @retval      ??
 */
void atk_rgblcd_show_char(uint16_t x, uint16_t y, char ch, atk_rgblcd_lcd_font_t font, uint16_t color) {
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

	ch_offset = ch - ' ';

	switch (font) {
#if (ATK_RGBLCD_FONT_12 != 0)
		case ATK_RGBLCD_LCD_FONT_12: {
			ch_code = atk_rgblcd_font_1206[ch_offset];
			ch_width = ATK_RGBLCD_FONT_12_CHAR_WIDTH;
			ch_height = ATK_RGBLCD_FONT_12_CHAR_HEIGHT;
			ch_size = ATK_RGBLCD_FONT_12_CHAR_SIZE;
			break;
		}
#endif
#if (ATK_RGBLCD_FONT_16 != 0)
		case ATK_RGBLCD_LCD_FONT_16: {
			ch_code = atk_rgblcd_font_1608[ch_offset];
			ch_width = ATK_RGBLCD_FONT_16_CHAR_WIDTH;
			ch_height = ATK_RGBLCD_FONT_16_CHAR_HEIGHT;
			ch_size = ATK_RGBLCD_FONT_16_CHAR_SIZE;
			break;
		}
#endif
#if (ATK_RGBLCD_FONT_24 != 0)
		case ATK_RGBLCD_LCD_FONT_24: {
			ch_code = atk_rgblcd_font_2412[ch_offset];
			ch_width = ATK_RGBLCD_FONT_24_CHAR_WIDTH;
			ch_height = ATK_RGBLCD_FONT_24_CHAR_HEIGHT;
			ch_size = ATK_RGBLCD_FONT_24_CHAR_SIZE;
			break;
		}
#endif
#if (ATK_RGBLCD_FONT_32 != 0)
		case ATK_RGBLCD_LCD_FONT_32: {
			ch_code = atk_rgblcd_font_3216[ch_offset];
			ch_width = ATK_RGBLCD_FONT_32_CHAR_WIDTH;
			ch_height = ATK_RGBLCD_FONT_32_CHAR_HEIGHT;
			ch_size = ATK_RGBLCD_FONT_32_CHAR_SIZE;
			break;
		}
#endif
		default: {
			return;
		}
	}

	if ((x + ch_width > ATK_RGBLCD_LCD_WIDTH) || (y + ch_height > ATK_RGBLCD_LCD_HEIGHT)) {
		return;
	}

	for (byte_index = 0; byte_index < ch_size; byte_index++) {
		byte_code = ch_code[byte_index];
		for (bit_index = 0; bit_index < 8; bit_index++) {
			if ((byte_code & 0x80) != 0) {
				atk_rgblcd_draw_point(x + width_index, y + height_index, color);
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

/**
 * @brief       RGB LCD???LCD????????
 * @note        ?????????????
 * @param       x     : ????????????X????
 *              y     : ????????????Y????
 *              width : ??????????????????
 *              height: ???????????????????
 *              str   : ??????????
 *              font  : ????????????????
 *              color : ???????????????
 * @retval      ??
 */
void atk_rgblcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char* str,
							atk_rgblcd_lcd_font_t font, uint16_t color) {
	uint8_t ch_width;
	uint8_t ch_height;
	uint16_t x_raw;
	uint16_t y_raw;
	uint16_t x_limit;
	uint16_t y_limit;

	switch (font) {
#if (ATK_RGBLCD_FONT_12 != 0)
		case ATK_RGBLCD_LCD_FONT_12: {
			ch_width = ATK_RGBLCD_FONT_12_CHAR_WIDTH;
			ch_height = ATK_RGBLCD_FONT_12_CHAR_HEIGHT;
			break;
		}
#endif
#if (ATK_RGBLCD_FONT_16 != 0)
		case ATK_RGBLCD_LCD_FONT_16: {
			ch_width = ATK_RGBLCD_FONT_16_CHAR_WIDTH;
			ch_height = ATK_RGBLCD_FONT_16_CHAR_HEIGHT;
			break;
		}
#endif
#if (ATK_RGBLCD_FONT_24 != 0)
		case ATK_RGBLCD_LCD_FONT_24: {
			ch_width = ATK_RGBLCD_FONT_24_CHAR_WIDTH;
			ch_height = ATK_RGBLCD_FONT_24_CHAR_HEIGHT;
			break;
		}
#endif
#if (ATK_RGBLCD_FONT_32 != 0)
		case ATK_RGBLCD_LCD_FONT_32: {
			ch_width = ATK_RGBLCD_FONT_32_CHAR_WIDTH;
			ch_height = ATK_RGBLCD_FONT_32_CHAR_HEIGHT;
			break;
		}
#endif
		default: {
			return;
		}
	}

	x_raw = x;
	y_raw = y;
	x_limit = ((x + width + 1) > ATK_RGBLCD_LCD_WIDTH) ? ATK_RGBLCD_LCD_WIDTH : (x + width + 1);
	y_limit = ((y + height + 1) > ATK_RGBLCD_LCD_HEIGHT) ? ATK_RGBLCD_LCD_HEIGHT : (y + height + 1);

	while ((*str >= ' ') && (*str <= '~')) {
		if (x + ch_width >= x_limit) {
			x = x_raw;
			y += ch_height;
		}

		if (y + ch_height >= y_limit) {
			y = x_raw;
			x = y_raw;
		}

		atk_rgblcd_show_char(x, y, *str, font, color);

		x += ch_width;
		str++;
	}
}

/**
 * @brief       RGB LCD???LCD????????????????????0
 * @param       x    : ??????????X????
 *              y    : ??????????Y????
 *              num  : ?????????
 *              len  : ??????????????
 *              mode : ATK_RGBLCD_NUM_SHOW_NOZERO: ???????0?????
 *                     ATK_RGBLCD_NUM_SHOW_ZERO  : ???????0???
 *              font : ??????????????
 *              color: ?????????????
 * @retval      ??
 */
void atk_rgblcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_rgblcd_num_mode_t mode,
						  atk_rgblcd_lcd_font_t font, uint16_t color) {
	uint8_t ch_width;
	uint8_t len_index;
	uint8_t num_index;
	uint8_t first_nozero = 0;
	char pad;

	switch (font) {
#if (ATK_RGBLCD_FONT_12 != 0)
		case ATK_RGBLCD_LCD_FONT_12: {
			ch_width = ATK_RGBLCD_FONT_12_CHAR_WIDTH;
			break;
		}
#endif
#if (ATK_RGBLCD_FONT_16 != 0)
		case ATK_RGBLCD_LCD_FONT_16: {
			ch_width = ATK_RGBLCD_FONT_16_CHAR_WIDTH;
			break;
		}
#endif
#if (ATK_RGBLCD_FONT_24 != 0)
		case ATK_RGBLCD_LCD_FONT_24: {
			ch_width = ATK_RGBLCD_FONT_24_CHAR_WIDTH;
			break;
		}
#endif
#if (ATK_RGBLCD_FONT_32 != 0)
		case ATK_RGBLCD_LCD_FONT_32: {
			ch_width = ATK_RGBLCD_FONT_32_CHAR_WIDTH;
			break;
		}
#endif
		default: {
			return;
		}
	}

	switch (mode) {
		case ATK_RGBLCD_NUM_SHOW_NOZERO: {
			pad = ' ';
			break;
		}
		case ATK_RGBLCD_NUM_SHOW_ZERO: {
			pad = '0';
			break;
		}
		default: {
			return;
		}
	}

	for (len_index = 0; len_index < len; len_index++) {
		num_index = (num / atk_rgblcd_pow(10, len - len_index - 1)) % 10;
		if ((first_nozero == 0) && (len_index < (len - 1))) {
			if (num_index == 0) {
				atk_rgblcd_show_char(x + ch_width * len_index, y, pad, font, color);
				continue;
			} else {
				first_nozero = 1;
			}
		}

		atk_rgblcd_show_char(x + ch_width * len_index, y, num_index + '0', font, color);
	}
}

/**
 * @brief       RGB LCD???LCD?????????????????0
 * @param       x    : ??????????X????
 *              y    : ??????????Y????
 *              num  : ?????????
 *              len  : ??????????????
 *              font : ??????????????
 *              color: ?????????????
 * @retval      ??
 */
void atk_rgblcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_rgblcd_lcd_font_t font,
						 uint16_t color) {
	atk_rgblcd_show_xnum(x, y, num, len, ATK_RGBLCD_NUM_SHOW_NOZERO, font, color);
}
