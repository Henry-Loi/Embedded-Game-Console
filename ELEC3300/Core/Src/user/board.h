#pragma once

#include "gpio.h"

#include "stm32f4xx.h"

#define gpio_set(gpio)	  HAL_GPIO_WritePin(gpio##_GPIO_Port, gpio##_Pin, GPIO_PIN_SET)
#define gpio_reset(gpio)  HAL_GPIO_WritePin(gpio##_GPIO_Port, gpio##_Pin, GPIO_PIN_RESET)
#define gpio_toggle(gpio) HAL_GPIO_TogglePin(gpio##_GPIO_Port, gpio##_Pin)
#define gpio_read(gpio)	  HAL_GPIO_ReadPin(gpio##_GPIO_Port, gpio##_Pin)

#define led_on(led)		gpio_reset(led) // changed from set to reset
#define led_off(led)	gpio_set(led)	// changed from reset to set
#define led_toggle(led) gpio_toggle(led)

/* Board GPIO defines */
#define LED1_GPIO_Port GPIOB
#define LED1_Pin	   GPIO_PIN_1

#define LED2_GPIO_Port GPIOB
#define LED2_Pin	   GPIO_PIN_0
