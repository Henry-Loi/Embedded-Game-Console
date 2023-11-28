/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : fatfs_platform.c
 * @brief          : fatfs_platform source file
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
#include "fatfs_platform.h"

#include "board.h"
#include "lcd.h"

uint8_t BSP_PlatformIsDetected(void) {
	uint8_t status = SD_PRESENT;
	/* Check SD card detect pin */
	if (HAL_GPIO_ReadPin(SD_DETECT_GPIO_Port, SD_DETECT_Pin) != GPIO_PIN_RESET) {
		status = SD_NOT_PRESENT;
	}
	/* USER CODE BEGIN 1 */
	/* user code can be inserted here */
	// status = SD_PRESENT;
	/* USER CODE END 1 */
	return status;
}

int fatfs_tft(int r) {
	tft_prints(0, r++, "SD Card Inserted: %d", BSP_PlatformIsDetected());
	return r;
}
