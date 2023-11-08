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

uint8_t retSDRAMDISK;  /* Return value for SDRAMDISK */
char SDRAMDISKPath[4]; /* SDRAMDISK logical drive path */
FATFS SDRAMDISKFatFS;  /* File system object for SDRAMDISK logical drive */
FIL SDRAMDISKFile;	   /* File object for SDRAMDISK */
uint8_t retSD;		   /* Return value for SD */
char SDPath[4];		   /* SD logical drive path */
FATFS SDFatFS;		   /* File system object for SD logical drive */
FIL SDFile;			   /* File object for SD */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void) {
	/*## FatFS: Link the SDRAMDISK driver ###########################*/
	retSDRAMDISK = FATFS_LinkDriver(&SDRAMDISK_Driver, SDRAMDISKPath);
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
DWORD get_fattime(void) {
	/* USER CODE BEGIN get_fattime */
	return 0;
	/* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
UINT fatfs_file_system_test(void) {
	// FATFS
	FATFS fs;	   /* FatFs file system object */
	FIL file;	   /* file object */
	FRESULT f_res; /* file operation result */
	UINT fnum;	   /* number of successful file operations */
	BYTE ReadBuffer[1024] = {0};
	BYTE WriteBuffer[] = "This is STM32 working with FatFs\r\n";
	f_res = f_mount(&fs, "0:", 1);

	/*----------------------- formatting test ---------------------------*/
	printf("\r\n****** Register the file system object to the FatFs module ******\r\n");

	/* 如果没有文件系统就格式化创建创建文件系统 */
	if (f_res == FR_NO_FILESYSTEM) {
		printf("The SD card does not yet have a file system and is about to be formatted...\r\n");
		/* formatting */
		f_res = f_mkfs("0:", 0, 0);
		if (f_res == FR_OK) {
			printf("The SD card successfully formatted the file system\r\n");
			/* 格式化后，先取消挂载 */
			f_res = f_mount(NULL, "0:", 1);
			/* 重新挂载 */
			f_res = f_mount(&fs, "0:", 1);
		} else {
			printf("The format failed\r\n");
			while (1)
				;
		}
	} else if (f_res != FR_OK) {
		printf(" mount error : %d \r\n", f_res);
		while (1)
			;
	} else {
		printf(" mount success!!! \r\n");
	}

	/*----------------------- write test -----------------------------*/
	/* 打开文件，如果文件不存在则创建它 */
	printf("\r\n****** Create and Open new text file objects with write access ******\r\n");
	f_res = f_open(&file, "0:FatFs STM32cube.txt", FA_CREATE_ALWAYS | FA_WRITE);

	if (f_res == FR_OK) {
		printf(" open file sucess!!! \r\n");
		/* 将指定存储区内容写入到文件内 */
		printf("\r\n****** Write data to the text files ******\r\n");
		f_res = f_write(&file, WriteBuffer, sizeof(WriteBuffer), &fnum);
		if (f_res == FR_OK) {
			printf(" write file success!!! (%d)\n", fnum);
			printf(" write Data : %s\r\n", WriteBuffer);
		} else {
			printf(" write file error : %d\r\n", f_res);
		}
		/* 不再读写，关闭文件 */
		f_close(&file);
	} else {
		printf(" open file error : %d\r\n", f_res);
	}

	/*------------------- 文件系统测试：读测试 ------------------------------------*/
	printf("\r\n****** Read data from the text files ******\r\n");
	f_res = f_open(&file, "0:FatFs STM32cube.txt", FA_OPEN_EXISTING | FA_READ);
	if (f_res == FR_OK) {
		printf(" open file success!!! \r\n");
		f_res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum);
		if (f_res == FR_OK) {
			printf("read success!!! (%d)\n", fnum);
			printf("read Data : %s\r\n", ReadBuffer);
		} else {
			printf(" read error!!! %d\r\n", f_res);
		}
	} else {
		printf(" open file error : %d\r\n", f_res);
	}
	/* 不再读写，关闭文件 */
	f_close(&file);
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL, "0:", 1);
	/* 操作完成，停机 */
}
/* USER CODE END Application */
