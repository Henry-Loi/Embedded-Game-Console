/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    ltdc.c
 * @brief   This file provides code for the configuration
 *          of the LTDC instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"

#include "board.h"
#include "dma2d.h"

#include "stm32f4xx_hal_ltdc.h"


/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc = {0};

uint8_t get_lcd_id(void) {
	uint8_t id;
	id = gpio_read(RGBLCD_M0);
	id |= gpio_read(RGBLCD_M1) << 1;
	id |= gpio_read(RGBLCD_M2) << 2;
}

/* LTDC init function */
void LTDC_INIT(void) {
	uint8_t id;

	id = get_lcd_id();

	MX_LTDC_Init();
	MX_DMA2D_Init();
}


void MX_LTDC_Init(void) {
	/* USER CODE BEGIN LTDC_Init 0 */

	/* USER CODE END LTDC_Init 0 */

	LTDC_LayerCfgTypeDef pLayerCfg = {0};
	LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

	/* USER CODE BEGIN LTDC_Init 1 */

	/* USER CODE END LTDC_Init 1 */
	hltdc.Instance = LTDC;
	hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
	hltdc.Init.HorizontalSync = 47;
	hltdc.Init.VerticalSync = 2;
	hltdc.Init.AccumulatedHBP = 135;
	hltdc.Init.AccumulatedVBP = 34;
	hltdc.Init.AccumulatedActiveW = 935;
	hltdc.Init.AccumulatedActiveH = 514;
	hltdc.Init.TotalWidth = 975;
	hltdc.Init.TotalHeigh = 527;
	hltdc.Init.Backcolor.Blue = 0;
	hltdc.Init.Backcolor.Green = 0;
	hltdc.Init.Backcolor.Red = 0;
	if (HAL_LTDC_Init(&hltdc) != HAL_OK) {
		Error_Handler();
	}
	pLayerCfg.WindowX0 = 0;
	pLayerCfg.WindowX1 = 1024;
	pLayerCfg.WindowY0 = 0;
	pLayerCfg.WindowY1 = 600;
	pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
	pLayerCfg.Alpha = 255;
	pLayerCfg.Alpha0 = 0;
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
	pLayerCfg.FBStartAdress = 0xC0000000;
	pLayerCfg.ImageWidth = 1024;
	pLayerCfg.ImageHeight = 600;
	pLayerCfg.Backcolor.Blue = 0;
	pLayerCfg.Backcolor.Green = 0;
	pLayerCfg.Backcolor.Red = 0;

	if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK) {
		Error_Handler();
	}
	pLayerCfg1.WindowX0 = 0;
	pLayerCfg1.WindowX1 = 1024;
	pLayerCfg1.WindowY0 = 0;
	pLayerCfg1.WindowY1 = 600;
	pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
	pLayerCfg1.Alpha = 255;
	pLayerCfg1.Alpha0 = 0;
	pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	pLayerCfg1.FBStartAdress = 0xC0200000;
	pLayerCfg1.ImageWidth = 1024;
	pLayerCfg1.ImageHeight = 600;
	pLayerCfg1.Backcolor.Blue = 0;
	pLayerCfg1.Backcolor.Green = 0;
	pLayerCfg1.Backcolor.Red = 0;
	if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, LTDC_LAYER_1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN LTDC_Init 2 */

	/* USER CODE END LTDC_Init 2 */
}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	if (ltdcHandle->Instance == LTDC) {
		/* USER CODE BEGIN LTDC_MspInit 0 */

		/* USER CODE END LTDC_MspInit 0 */

		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
		PeriphClkInitStruct.PLLSAI.PLLSAIN = 360;
		PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
		PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
			Error_Handler();
		}

		/* LTDC clock enable */
		__HAL_RCC_LTDC_CLK_ENABLE();

		__HAL_RCC_GPIOI_CLK_ENABLE();
		__HAL_RCC_GPIOF_CLK_ENABLE();
		__HAL_RCC_GPIOH_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();
		/**LTDC GPIO Configuration
		PI9     ------> LTDC_VSYNC
		PI10     ------> LTDC_HSYNC
		PF10     ------> LTDC_DE
		PH9     ------> LTDC_R3
		PH10     ------> LTDC_R4
		PH11     ------> LTDC_R5
		PH12     ------> LTDC_R6
		PG6     ------> LTDC_R7
		PG7     ------> LTDC_CLK
		PH13     ------> LTDC_G2
		PH14     ------> LTDC_G3
		PH15     ------> LTDC_G4
		PI0     ------> LTDC_G5
		PI1     ------> LTDC_G6
		PI2     ------> LTDC_G7
		PG11     ------> LTDC_B3
		PI4     ------> LTDC_B4
		PI5     ------> LTDC_B5
		PI6     ------> LTDC_B6
		PI7     ------> LTDC_B7
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5
							  | GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

		GPIO_InitStruct.Pin =
			GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		/* USER CODE BEGIN LTDC_MspInit 1 */

		/* USER CODE END LTDC_MspInit 1 */
	}
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* ltdcHandle) {
	if (ltdcHandle->Instance == LTDC) {
		/* USER CODE BEGIN LTDC_MspDeInit 0 */

		/* USER CODE END LTDC_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_LTDC_CLK_DISABLE();

		/**LTDC GPIO Configuration
		PI9     ------> LTDC_VSYNC
		PI10     ------> LTDC_HSYNC
		PF10     ------> LTDC_DE
		PH9     ------> LTDC_R3
		PH10     ------> LTDC_R4
		PH11     ------> LTDC_R5
		PH12     ------> LTDC_R6
		PG6     ------> LTDC_R7
		PG7     ------> LTDC_CLK
		PH13     ------> LTDC_G2
		PH14     ------> LTDC_G3
		PH15     ------> LTDC_G4
		PI0     ------> LTDC_G5
		PI1     ------> LTDC_G6
		PI2     ------> LTDC_G7
		PG11     ------> LTDC_B3
		PI4     ------> LTDC_B4
		PI5     ------> LTDC_B5
		PI6     ------> LTDC_B6
		PI7     ------> LTDC_B7
		*/
		HAL_GPIO_DeInit(GPIOI, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5
								   | GPIO_PIN_6 | GPIO_PIN_7);

		HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10);

		HAL_GPIO_DeInit(GPIOH,
						GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

		HAL_GPIO_DeInit(GPIOG, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11);

		/* USER CODE BEGIN LTDC_MspDeInit 1 */

		/* USER CODE END LTDC_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
