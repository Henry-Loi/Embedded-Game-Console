/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file   fatfs.c
 * @brief  Code for fatfs applications
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
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
	/* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
	return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
UINT fatfs_file_system_test(int r) {
	// FATFS
	FATFS fs;	   /* FatFs file system object */
	FIL file;	   /* file object */
	FRESULT f_res; /* file operation result */
	UINT fnum;	   /* number of successful file operations */
	BYTE ReadBuffer[1024] = {0};
	BYTE WriteBuffer[] = "This is STM32 working with FatFs\r\n";
	f_res = f_mount(&fs, "0:", 1);

	/*----------------------- file system formatting test ---------------------------*/
	tft_prints(0, r++, "\r\n****** Register the file system object to the FatFs module ******\r\n");

	/* if no file system then create one */
	if (f_res == FR_NO_FILESYSTEM) {
		tft_prints(0, r++, "The SD card does not yet have a file system and is about to be formatted...\r\n");
		/* formatting */
		f_res = f_mkfs("0:", 0, 0, NULL, 0);
		if (f_res == FR_OK) {
			tft_prints(0, r++, "The SD card successfully formatted the file system\r\n");
			/* unmount object after formatting */
			f_res = f_mount(NULL, "0:", 1);
			/* remount object */
			f_res = f_mount(&fs, "0:", 1);
		} else {
			tft_prints(0, r++, "The format failed\r\n");
			// while (1)
			// 	;
		}
	} else if (f_res != FR_OK) {
		tft_prints(0, r++, " mount error : %d \r\n", f_res);
		// while (1)
		// 	;
	} else {
		tft_prints(0, r++, " mount success!!! \r\n");
	}

	/*----------------------- file system write test -----------------------------*/
	/* open a file, if file doesn't exit then create it */
	tft_prints(0, r++, "\r\n****** Create and Open new text file objects with write access ******\r\n");
	f_res = f_open(&file, "0:FatFs STM32cube.txt", FA_CREATE_ALWAYS | FA_WRITE);

	if (f_res == FR_OK) {
		tft_prints(0, r++, " open file sucess!!! \r\n");
		/* write specified sector content into file */
		tft_prints(0, r++, "\r\n****** Write data to the text files ******\r\n");
		f_res = f_write(&file, WriteBuffer, sizeof(WriteBuffer), &fnum);
		if (f_res == FR_OK) {
			tft_prints(0, r++, " write file success!!! (%d)\n", fnum);
			tft_prints(0, r++, " write Data : %s\r\n", WriteBuffer);
		} else {
			tft_prints(0, r++, " write file error : %d\r\n", f_res);
		}
		/* close file */
		f_close(&file);
	} else {
		tft_prints(0, r++, " open file error : %d\r\n", f_res);
	}

	/*------------------- file system read test ------------------------------------*/
	tft_prints(0, r++, "\r\n****** Read data from the text files ******\r\n");
	f_res = f_open(&file, "0:FatFs STM32cube.txt", FA_OPEN_EXISTING | FA_READ);
	if (f_res == FR_OK) {
		tft_prints(0, r++, " open file success!!! \r\n");
		f_res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum);
		if (f_res == FR_OK) {
			tft_prints(0, r++, "read success!!! (%d)\n", fnum);
			tft_prints(0, r++, "read Data : %s\r\n", ReadBuffer);
		} else {
			tft_prints(0, r++, " read error!!! %d\r\n", f_res);
		}
	} else {
		tft_prints(0, r++, " open file error : %d\r\n", f_res);
	}
	/* stop write-read operation, close the file */
	f_close(&file);
	/* unmount the file object */
	f_mount(NULL, "0:", 1);
	/* test done */
}
/* USER CODE END Application */
