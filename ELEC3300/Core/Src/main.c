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

#include "dma2d.h"
#include "fmc.h"
#include "gpio.h"
#include "ltdc.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lv_100ask_modules/lv_100ask_modules.h"
#include "lv_conf.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
// #include "lv_lib_png/lv_png.h"
#include "lvgl/lvgl.h"
#include "user/lcd.h"
#include "user/sdram.h"


#define DISP_BUF_SIZE (1024 * 600)

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define EXT_SDRAM_ADDR ((uint32_t)0xC0000000)
#define EXT_SDRAM_SIZE (32 * 1024 * 1024)

uint32_t bsp_TestExtSDRAM(void);
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
	MX_LTDC_Init();
	MX_FMC_Init();
	MX_DMA2D_Init();
	/* USER CODE BEGIN 2 */
	uint32_t last_ticks = 0;
	SDRAM_Init();
	lcd_init();

	lv_init();
	// fbdev_init();

	/*Linux frame buffer device init*/
	// fbdev_init();

	/*A small buffer for LittlevGL to draw the screen's content*/
	static lv_color_t buf[DISP_BUF_SIZE];

	/*Initialize a descriptor for the buffer*/
	static lv_disp_draw_buf_t disp_buf;
	lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

	/*Initialize and register a display driver*/
	static lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.draw_buf = &disp_buf;
	// disp_drv.flush_cb = fbdev_flush;
	disp_drv.hor_res = 1024;
	disp_drv.ver_res = 600;
	lv_disp_drv_register(&disp_drv);

	/* Linux input device init */
	// evdev_init();

	/* Initialize and register a display input driver */
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv); /*Basic initialization*/

	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = NULL; // lv_gesture_dir_t lv_indev_get_gesture_dir(const lv_indev_t * indev)
	lv_indev_t* my_indev = lv_indev_drv_register(&indev_drv);

	// 支持png
	// lv_png_init();

	// Set Image Cache size
	lv_img_cache_set_size(32);

	// 调用进程间通信管理初始化函数
	// lv_100ask_dbus_handler_init("net.ask100.lvgl.Main", "/net/ask100/lvgl/Main");

	/* 初始化桌面环境 */
	// lv_100ask_boot_animation(lv_100ask_demo_init_icon, 1500); //开机动画
	// lv_100ask_demo_init_icon();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		// tft_prints(0, 0, "%d", HAL_GetTick());
		// tft_prints(0, 1, "This is a test");
		// tft_prints(0, 2, "%d	%0.3f	%c	%s", 108, 0.05, 'a', "hi");
		// tft_draw_line(0, 0, 1024, 600, RED);
		// tft_draw_line(0, 600, 1024, 0, GREEN);

		// if (HAL_GetTick() - last_ticks > 100) {
		// 	last_ticks = HAL_GetTick();
		// 	tft_update();
		// 	HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
		// }
		lv_task_handler();
		/* USER CODE END 3 */
	}
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
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 15;
	RCC_OscInitStruct.PLL.PLLN = 216;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Activate the Over-Drive mode
	 */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
