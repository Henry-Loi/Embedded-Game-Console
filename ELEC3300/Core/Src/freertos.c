/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"

#include "main.h"
#include "task.h"

#include "cmsis_os.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "board.h"
#include "controller.h"
#include "games/asteroids/asteroids_thread.h"
#include "games/pong/pong.h"
#include "games/tetris/screen.h"
#include "gpio.h"
#include "icm20602.h"
#include "lv_hal_tick.h"
#include "os.h"
#include "serial.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
DEFINE_THREAD_ATTR(led_task, osPriorityNormal);
DEFINE_THREAD_ATTR_SIZED(lcd_task, 4096, osPriorityAboveNormal);
DEFINE_THREAD_ATTR(debug_task, 2);
DEFINE_THREAD_ATTR(controller_task, osPriorityAboveNormal);
DEFINE_THREAD_ATTR(imu_task, osPriorityAboveNormal);
DEFINE_THREAD_ATTR_SIZED(asteroids_task, 4096, osPriorityRealtime);
DEFINE_THREAD_ATTR_SIZED(tetris_task, 1024, osPriorityRealtime2);
DEFINE_THREAD_ATTR_SIZED(pong_task, 1024, osPriorityRealtime3);

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
	.name = "defaultTask",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void* argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationTickHook(void);

/* USER CODE BEGIN 3 */
void led_blinky(void* par) {
	uint32_t last_ticks = 0;

	while (1) {
		osDelay(4);
		if (HAL_GetTick() - last_ticks >= 100) {
			led_toggle(LED1);
			led_toggle(LED2);
			led_toggle(LED3);
			led_toggle(LED4);
			led_toggle(LED5);
			led_toggle(LED6);
			led_toggle(LED7);
			// led_toggle(LED8);
			last_ticks = HAL_GetTick();
		}
	}
}


void vApplicationTickHook(void) {
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
	// lv_tick_inc(1);
}

void lcd_thread(void* par);
/* USER CODE END 3 */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	CREATE_THREAD(led_task, led_blinky, NULL);
	CREATE_THREAD(lcd_task, lcd_thread, NULL);
	// CREATE_THREAD(debug_task, debug_thread, NULL);
	CREATE_THREAD(controller_task, controller_thread, NULL);
	// CREATE_THREAD(imu_task, imu_thread, NULL);
	CREATE_THREAD(asteroids_task, asteroids_thread, NULL);
	CREATE_THREAD(tetris_task, tetris_thread, NULL);
	CREATE_THREAD(pong_task, pong_thread, NULL);

	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void* argument) {
	/* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
