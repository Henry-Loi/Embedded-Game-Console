/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"

#include "adc.h"
#include "dma.h"
#include "dma2d.h"
#include "fatfs.h"
#include "fmc.h"
#include "gpio.h"
#include "ltdc.h"
#include "sdio.h"

#include "cmsis_os.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "board.h"
#include "gpio.h"
#include "os.h"
#include "sdram.h"
#include "user/display/touch.h"


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

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
// SDRAM
#define EXT_SDRAM_ADDR ((uint32_t)0xC0000000)
#define EXT_SDRAM_SIZE (32 * 1024 * 1024)

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

DEFINE_THREAD(led_task, led_blinky);

void led_blinky(void* par) {
	uint32_t last_ticks = 0;

	while (1) {
		osDelay(4);
		if (HAL_GetTick() - last_ticks >= 100) {
			gpio_toggle(LED1);
			last_ticks = HAL_GetTick();
		}
	}
}

DEFINE_THREAD_SIZED(lcd_task, lcd_thread, 4096);
DEFINE_THREAD(controller_task, buttons_handler);
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */


	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_LTDC_Init();
	MX_FMC_Init();
	MX_DMA2D_Init();
	MX_ADC1_Init();
	MX_SDIO_SD_Init();
	MX_FATFS_Init();
	/* USER CODE BEGIN 2 */

	SDRAM_Init();

	// FATFS
	// fatfs_file_system_test();

	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize(); /* Call init function for freertos objects (in freertos.c) */

	os_create_thread(led_task, NULL, 4);
	os_create_thread(lcd_task, NULL, 1);
	os_create_thread(controller_task, NULL, 3);

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	uint32_t temp = 0;
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		// tft_prints(0, 0, "%d", HAL_GetTick());
		// tft_prints(0, 11, "TEST");7
		// tft_prints(0, 1, "LAST %d", touch_feedback.last_pressed_state);
		// tft_prints(0, 2, "CURR %d", touch_feedback.pressed_state);
		// tft_prints(0, 3, "TEST: %d", touch_feedback.clicked);

		tft_prints(0, 4, "x: %d", touch_feedback.point[0].x);
		tft_prints(0, 5, "y: %d", touch_feedback.point[0].y);
		tft_prints(0, 6, "another test: %d", temp);
		tft_prints(0, 5, "ddd: %d", touch_feedback.clicked);
		temp += touch_feedback.clicked;

		// tft_prints(0, 14, "%d", touch_feedback.state == 28);


		// tft_prints(0, 1, "Joy L > x: %d y: %d btn: %d", joy_adc_values[0], joy_adc_values[1],
		// 		   HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6));
		// tft_prints(0, 2, "Joy R > x: %d y: %d btn: %d", joy_adc_values[2], joy_adc_values[3],
		// 		   HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4));

		// for (int i = 0; i < MAX_TOUCH_POINTS; i++) {
		// 	tft_prints(0, 3 + i * 3, "Point %d: ", i);
		// 	tft_prints(0, 4 + i * 3, "x: %d", touch_feedback.point[i].x);
		// 	tft_prints(0, 5 + i * 3, "y: %d", touch_feedback.point[i].y);
		// }


		if (HAL_GetTick() - last_ticks > 100) {
			last_ticks = HAL_GetTick();
			// tft_update();
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		}
		lv_task_handler();
		touch_update();
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 15;
	RCC_OscInitStruct.PLL.PLLN = 144;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 5;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM1) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
