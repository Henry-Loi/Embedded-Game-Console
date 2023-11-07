/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SD_Card_Check_Pin		GPIO_PIN_2
#define SD_Card_Check_GPIO_Port GPIOE
#define Joy_R_Btn_Pin			GPIO_PIN_4
#define Joy_R_Btn_GPIO_Port		GPIOE
#define Joy_L_Btn_Pin			GPIO_PIN_6
#define Joy_L_Btn_GPIO_Port		GPIOE
#define TP_CS_Pin				GPIO_PIN_8
#define TP_CS_GPIO_Port			GPIOI
#define LED1_Pin				GPIO_PIN_1
#define LED1_GPIO_Port			GPIOB
#define TP_SCK_Pin				GPIO_PIN_6
#define TP_SCK_GPIO_Port		GPIOH
#define TP_PEN_Pin				GPIO_PIN_7
#define TP_PEN_GPIO_Port		GPIOH
#define TP_SDA_Pin				GPIO_PIN_3
#define TP_SDA_GPIO_Port		GPIOI
#define LCD_BL_Pin				GPIO_PIN_5
#define LCD_BL_GPIO_Port		GPIOB
/* USER CODE BEGIN Private defines */
// #define HAS_SD_INSERT_CHECK /* uncomment if SD Insert pin is present */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
