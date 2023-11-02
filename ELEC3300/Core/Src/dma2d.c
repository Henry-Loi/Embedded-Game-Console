/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    dma2d.c
 * @brief   This file provides code for the configuration
 *          of the DMA2D instances.
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
#include "dma2d.h"

/* USER CODE BEGIN 0 */
#include "user/ltdc_driver.h"
/* USER CODE END 0 */

DMA2D_HandleTypeDef hdma2d;

/* DMA2D init function */
void MX_DMA2D_Init(void) {
	/* USER CODE BEGIN DMA2D_Init 0 */

	/* USER CODE END DMA2D_Init 0 */

	/* USER CODE BEGIN DMA2D_Init 1 */

	/* USER CODE END DMA2D_Init 1 */
	hdma2d.Instance = DMA2D;
	hdma2d.Init.Mode = DMA2D_R2M;
	hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
	hdma2d.Init.OutputOffset = 0;
	if (HAL_DMA2D_Init(&hdma2d) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN DMA2D_Init 2 */

	/* USER CODE END DMA2D_Init 2 */
}

void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef* dma2dHandle) {
	if (dma2dHandle->Instance == DMA2D) {
		/* USER CODE BEGIN DMA2D_MspInit 0 */

		/* USER CODE END DMA2D_MspInit 0 */
		/* DMA2D clock enable */
		__HAL_RCC_DMA2D_CLK_ENABLE();
		/* USER CODE BEGIN DMA2D_MspInit 1 */

		/* USER CODE END DMA2D_MspInit 1 */
	}
}

void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef* dma2dHandle) {
	if (dma2dHandle->Instance == DMA2D) {
		/* USER CODE BEGIN DMA2D_MspDeInit 0 */

		/* USER CODE END DMA2D_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_DMA2D_CLK_DISABLE();
		/* USER CODE BEGIN DMA2D_MspDeInit 1 */

		/* USER CODE END DMA2D_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */
void dma2d_transfer_data_r2m(uint32_t* addr, uint32_t xSize, uint32_t ySize, uint32_t offsetLine, uint16_t color) {
	DMA2D->CR = DMA2D_R2M; // dma2d mode: register to memory.
	DMA2D->OPFCCR = DMA2D_OUTPUT_RGB565;

	DMA2D->OCOLR = color;
	DMA2D->OMAR = (uint32_t)addr;
	DMA2D->OOR = offsetLine;
	DMA2D->NLR = (uint32_t)(xSize << 16) | (uint16_t)ySize;

	DMA2D->CR |= DMA2D_CR_START;
	while (DMA2D->CR & DMA2D_CR_START)
		;
}

/* USER CODE END 1 */
