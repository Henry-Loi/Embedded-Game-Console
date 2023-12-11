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
#define TP_CS_Pin GPIO_PIN_8
#define TP_CS_GPIO_Port GPIOI
#define RBTN_LEFT_Pin GPIO_PIN_13
#define RBTN_LEFT_GPIO_Port GPIOC
#define LBTN_UP_Pin GPIO_PIN_6
#define LBTN_UP_GPIO_Port GPIOF
#define LBTN_DOWN_Pin GPIO_PIN_7
#define LBTN_DOWN_GPIO_Port GPIOF
#define LBTN_LEFT_Pin GPIO_PIN_8
#define LBTN_LEFT_GPIO_Port GPIOF
#define LBTN_RIGHT_Pin GPIO_PIN_9
#define LBTN_RIGHT_GPIO_Port GPIOF
#define RBTN_UP_Pin GPIO_PIN_0
#define RBTN_UP_GPIO_Port GPIOA
#define RBTN_DOWN_Pin GPIO_PIN_2
#define RBTN_DOWN_GPIO_Port GPIOH
#define RBTN_RIGHT_Pin GPIO_PIN_3
#define RBTN_RIGHT_GPIO_Port GPIOH
#define LED4_Pin GPIO_PIN_6
#define LED4_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_7
#define LED3_GPIO_Port GPIOA
#define Joy_L_Btn_Pin GPIO_PIN_4
#define Joy_L_Btn_GPIO_Port GPIOC
#define Joy_R_Btn_Pin GPIO_PIN_5
#define Joy_R_Btn_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOB
#define TP_SCK_Pin GPIO_PIN_6
#define TP_SCK_GPIO_Port GPIOH
#define TP_PEN_Pin GPIO_PIN_7
#define TP_PEN_GPIO_Port GPIOH
#define IMU_CS_Pin GPIO_PIN_12
#define IMU_CS_GPIO_Port GPIOB
#define IMU_INT_Pin GPIO_PIN_13
#define IMU_INT_GPIO_Port GPIOB
#define TP_SDA_Pin GPIO_PIN_3
#define TP_SDA_GPIO_Port GPIOI
#define SD_Card_Check_Pin GPIO_PIN_5
#define SD_Card_Check_GPIO_Port GPIOD
#define LED8_Pin GPIO_PIN_6
#define LED8_GPIO_Port GPIOB
#define LED7_Pin GPIO_PIN_7
#define LED7_GPIO_Port GPIOB
#define LED6_Pin GPIO_PIN_8
#define LED6_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_9
#define LED5_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
// #define HAS_SD_INSERT_CHECK /* uncomment if SD Insert pin is present */

#define __forceinline __attribute__((__always_inline__)) inline
#define get_ticks() HAL_GetTick()

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
