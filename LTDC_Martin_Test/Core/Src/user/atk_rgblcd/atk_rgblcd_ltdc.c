/**
 ****************************************************************************************************
 * @file        atk_rgblcd_ltdc.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-24
 * @brief       ����ԭ��RGB LCDģ��LTDC�ӿ���������
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

#include "atk_rgblcd_ltdc.h"

#include "ltdc.h"


/* RGB LCDģ��LTDC���? */
static LTDC_HandleTypeDef g_ltdc_handle = {0};

/**
 * @brief       RGB LCDģ��LTDC�ӿ�ʱ������
 * @note        LCD_CLK frequency = f(PLLSAI clock input) * PLLSAIN / PLLSAIR / PLLSAIDIVR
 * @param       ��
 * @retval      ��
 */
static void atk_rgblcd_ltdc_clock_config(void) {
	RCC_PeriphCLKInitTypeDef ltdc_clk_init_struct;

	ltdc_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	ltdc_clk_init_struct.PLLSAI.PLLSAIN = ATK_RGBLCD_LTDC_PLLSAIN;
	ltdc_clk_init_struct.PLLSAI.PLLSAIR = ATK_RGBLCD_LTDC_PLLSAIR;
	ltdc_clk_init_struct.PLLSAIDivR = ATK_RGBLCD_LTDC_PLLSAIDIVR;
	HAL_RCCEx_PeriphCLKConfig(&ltdc_clk_init_struct);
}

/**
 * @brief       RGB LCDģ��LTDC�ӿڳ�ʼ��
 * @param       ��
 * @retval      ��
 */
void atk_rgblcd_ltdc_init(uint16_t width, uint16_t height, const atk_rgblcd_timing_t* timing) {
	GPIO_InitTypeDef gpio_init_struct = {0};
	LTDC_LayerCfgTypeDef layer_init_struct = {0};

	/* ʹ��ʱ�� */
	ATK_RGBLCD_LTDC_CLK_ENABLE();
	ATK_RGBLCD_LTDC_DE_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_CLK_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_HSYNC_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_VSYNC_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_R3_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_R4_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_R5_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_R6_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_R7_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_G2_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_G3_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_G4_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_G5_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_G6_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_G7_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_B3_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_B4_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_B5_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_B6_GPIO_CLK_ENABLE();
	ATK_RGBLCD_LTDC_B7_GPIO_CLK_ENABLE();

	/* ��ʼ��DE���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_DE_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_DE_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_DE_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��CLK���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_CLK_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_CLK_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_CLK_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��HSYNC���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_HSYNC_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_HSYNC_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_HSYNC_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��VSYNC���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_VSYNC_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_VSYNC_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_VSYNC_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��R3���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_R3_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_R3_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_R3_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��R4���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_R4_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_R4_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_R4_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��R5���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_R5_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_R5_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_R5_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��R6���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_R6_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_R6_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_R6_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��R7���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_R7_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_R7_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_R7_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��G2���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_G2_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_G2_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_G2_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��G3���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_G3_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_G3_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_G3_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��G4���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_G4_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_G4_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_G4_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��G5���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_G5_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_G5_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_G5_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��G6���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_G6_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_G6_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_G6_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��G7���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_G7_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_G7_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_G7_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��B3���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_B3_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_B3_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_B3_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��B4���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_B4_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_B4_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_B4_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��B5���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_B5_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_B5_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_B5_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��B6���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_B6_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_B6_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_B6_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��B7���� */
	gpio_init_struct.Pin = ATK_RGBLCD_LTDC_B7_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_struct.Alternate = ATK_RGBLCD_LTDC_B7_GPIO_AF;
	HAL_GPIO_Init(ATK_RGBLCD_LTDC_B7_GPIO_PORT, &gpio_init_struct);

	/* LTDCʱ������ */
	atk_rgblcd_ltdc_clock_config();

	/* LTDC���� */
	g_ltdc_handle.Instance = LTDC;
	g_ltdc_handle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	g_ltdc_handle.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	g_ltdc_handle.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	g_ltdc_handle.Init.PCPolarity = (timing->pixel_clock_polarity == 0) ? LTDC_PCPOLARITY_IIPC : LTDC_PCPOLARITY_IPC;
	g_ltdc_handle.Init.HorizontalSync = timing->hsync_len - 1;
	g_ltdc_handle.Init.VerticalSync = timing->vsync_len - 1;
	g_ltdc_handle.Init.AccumulatedHBP = timing->hsync_len + timing->hback_porch - 1;
	g_ltdc_handle.Init.AccumulatedVBP = timing->vsync_len + timing->vback_porch - 1;
	g_ltdc_handle.Init.AccumulatedActiveW = timing->hsync_len + timing->hback_porch + timing->hactive - 1;
	g_ltdc_handle.Init.AccumulatedActiveH = timing->vsync_len + timing->vback_porch + timing->vactive - 1;
	g_ltdc_handle.Init.TotalWidth =
		timing->hsync_len + timing->hback_porch + timing->hactive + timing->hfront_porch - 1;
	g_ltdc_handle.Init.TotalHeigh =
		timing->vsync_len + timing->vback_porch + timing->vactive + timing->vfront_porch - 1;
	g_ltdc_handle.Init.Backcolor.Blue = 0;
	g_ltdc_handle.Init.Backcolor.Green = 0;
	g_ltdc_handle.Init.Backcolor.Red = 0;
	HAL_LTDC_Init(&g_ltdc_handle);

	/* LTDC������ */
	layer_init_struct.WindowX0 = 0;
	layer_init_struct.WindowX1 = width;
	layer_init_struct.WindowY0 = 0;
	layer_init_struct.WindowY1 = height;
	layer_init_struct.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
	layer_init_struct.Alpha = 255;
	layer_init_struct.Alpha0 = 0;
	layer_init_struct.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	layer_init_struct.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	layer_init_struct.FBStartAdress = ATK_RGBLCD_LTDC_LAYER_FB_ADDR;
	layer_init_struct.ImageWidth = width;
	layer_init_struct.ImageHeight = height;
	layer_init_struct.Backcolor.Blue = 0;
	layer_init_struct.Backcolor.Green = 0;
	layer_init_struct.Backcolor.Red = 0;
	HAL_LTDC_ConfigLayer(&g_ltdc_handle, &layer_init_struct, LTDC_LAYER_1);
}

/**
 * @brief       ʹ��RGB LCDģ��LTDC�ӿ�
 * @param       ��
 * @retval      ��
 */
void atk_rgblcd_ltdc_enable(void) { __HAL_LTDC_ENABLE(&g_ltdc_handle); }

/**
 * @brief       ����RGB LCDģ��LTDC�ӿ�
 * @param       ��
 * @retval      ��
 */
void atk_rgblcd_ltdc_disable(void) { __HAL_LTDC_DISABLE(&g_ltdc_handle); }
