/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "dma2d.h"
#include "fatfs.h"
#include "ltdc.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "BSP_RGB_LCD.h"
#include "image.h"
#include "GUI_Paint.h"
#include "GT911.h"
#include "malloc.h"	  
#include "usmart.h"
#include "ff.h"
#include "ff_gen_drv.h"	
#include "piclib.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern GT911_Dev Dev_Now;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_7)
	{
		Dev_Now.Touch = 1;
	}
}
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

//FATFS SDFatFs;  /* File system object for SD card logical drive */
//FIL MyFile;     /* File object */
//char SDPath[4]; /* SD card logical drive path */
//static uint8_t buffer[_MAX_SS]; /* a work buffer for the f_mkfs() */

uint16_t *picture = NULL;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
//  FRESULT res;                                          /* FatFs function common result code */
//  uint32_t byteswritten, bytesread;                     /* File write/read counts */
//  uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
//  uint8_t rtext[100];                                   /* File read buffer */
//	uint8_t buffer[512];
	
	
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
  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_SDIO_SD_Init();
//  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	
	/*LCD初始化*/
	BSP_LCD_Init();
	
	/*打开背光*/
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
	
/*malloc.c中初始化内存池*/
	my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMCCM);		    //初始化CCM内存池 
	
/*USMART调试工具初始化*/
	usmart_dev.init(90);
	

	/*使能串口1接受中断*/
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE); 
	
	/*为fatfs相关变量申请内存	*/
	exfuns_init();
	
	/*FATFS文件系统初始化，初始化SD卡并挂载SD卡*/
	if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
		{
			/*##-2- Register the file system object to the FatFs module ##############*/
			if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) != FR_OK)
			{
				/* FatFs Initialization Error */
				Error_Handler();
				Debug("挂载失败\r\n");
			}
			else
			{
				/*##-3- Create a FAT file system (format) on the logical drive #########*/
				/* WARNING: Formatting the uSD card will delete all content on the device */
				if(disk_initialize(0) != 0)//在这一步进行了SD卡初始化   f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, buffer, sizeof(buffer)) != FR_OK  disk_initialize(0) != 0
				{
					/* FatFs Format Error */
					Error_Handler();
					Debug("SD卡初始化失败\r\n");
				}
				else
				{       
					/*##-4- Create and Open a new text file object with write access #####*/
//					if(f_open(&SDFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
//					{
//						/* 'STM32.TXT' file Open for write Error */
//						Error_Handler();
//						Debug("创建文件失败\r\n");
//					}
//					else
//					{
//						/*##-5- Write data to the text file ################################*/
//						res = f_write(&SDFile, wtext, sizeof(wtext), (void *)&byteswritten);

//						/*##-6- Close the open text file #################################*/
//						if (f_close(&SDFile) != FR_OK )
//						{
//							Error_Handler();
//							Debug("关闭文件失败\r\n");
//						}
//						
//						if((byteswritten == 0) || (res != FR_OK))
//						{
//							/* 'STM32.TXT' file Write or EOF Error */
//							Error_Handler();
//							Debug("写文件信息失败\r\n");
//						}
//						else
//						{      
//							/*##-7- Open the text file object with read access ###############*/
//							if(f_open(&SDFile, "STM32.TXT", FA_READ) != FR_OK)
//							{
//								/* 'STM32.TXT' file Open for read Error */
//								Error_Handler();
//								Debug("文件打开失败\r\n");
//							}
//							else
//							{
//								/*##-8- Read data from the text file ###########################*/
//								res = f_read(&SDFile, rtext, sizeof(rtext), (UINT*)&bytesread);
//								
//								if((bytesread == 0) || (res != FR_OK))
//								{
//									/* 'STM32.TXT' file Read or EOF Error */
//									Error_Handler();
//									Debug("文件读取失败或EOF错误\r\n");
//								}
//								else
//								{
//									/*##-9- Close the open text file #############################*/
//									f_close(&SDFile);
//									
//									/*##-10- Compare read data with the expected data ############*/
//									if((bytesread != byteswritten))
//									{                
//										/* Read data is different from the expected data */
//										Error_Handler();
//										Debug("读数据不同\r\n");
//									}
//									else
//									{
										/* Success of the demo: no error occurrence */
										Debug("SD卡挂载并写入成功\r\n");
//									}
//								}
//							}
//						}
//					}
				}
			}
		}
		else
		{
			/* Read data is different from the expected data */
			Error_Handler();
			Debug("FATFS文件系统链接失败r\n");		
		}
		
		
	/*刷屏代码*/
	Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, ROTATE_0, WHITE);
	Paint_Clear(WHITE);
	Paint_DrawString_EN(10, 230, "1234567890", &Font24, 0x000f, 0xfff0);
	Paint_DrawString_EN(10, 260, "ABCDEFGHIJ", &Font24, BLUE, CYAN);
	Paint_DrawString_CN(10, 290, "微雪电子", &Font24CN, BLUE, CYAN);

	Paint_DrawRectangle(200, 230, 300, 330, RED,DRAW_FILL_EMPTY, DOT_PIXEL_2X2 );
	Paint_DrawLine(200, 230, 300, 330, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
	Paint_DrawLine(300, 230, 200, 330, MAGENTA, LINE_STYLE_SOLID, DOT_PIXEL_2X2);
	Paint_DrawCircle(250, 280, 45, GREEN, DRAW_FILL_EMPTY, DOT_PIXEL_2X2);
	Paint_DrawImage(gImage_800X221, 0, 0, 800, 221);
	Paint_DrawString_EN(715, 0, "clear", &Font24, BLUE, CYAN);/*每个字符占17个x坐标 24个Y坐标*/
	
	/*GT9147触摸IC初始化*/
	GT911_Init();
		
	/*解码图片*/	
//	if(f_open(&SDFile, "image.jpg", FA_READ) == FR_OK)
//	{
//		/* Decode the jpg image file */
//		Debug("文件打开成功\r\n");
//		jpeg_decode(&SDFile, 800, _aucLine, Jpeg_CallbackFunction);
		picture = (uint16_t*)mymalloc(SRAMEX,800*480*2);
		piclib_init();										//初始化画图	
		ai_load_picfile((const u8*)"0:/image2.jpg",0,0,800,480,1);//显示图片   
		
//		Paint_DrawImage((const unsigned char*)picture, 0, 0, 800, 480);
		
		myfree(SRAMEX,picture);
		/* Close the JPG image */
//		f_close(&SDFile);
//	}	
//	else
//	{
//		Debug("文件打开失败\r\n");
//	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

        		
		ctp_test();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 152;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
