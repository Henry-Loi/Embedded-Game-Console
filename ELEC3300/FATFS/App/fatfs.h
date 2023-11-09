/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file   fatfs.h
 * @brief  Header for fatfs applications
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
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "ff.h"
#include "ff_gen_drv.h"
#include "sdram_diskio.h" /* defines SDRAMDISK_Driver as external */
#include "sd_diskio.h" /* defines SD_Driver as external */

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern uint8_t retSDRAMDISK; /* Return value for SDRAMDISK */
extern char SDRAMDISKPath[4]; /* SDRAMDISK logical drive path */
extern FATFS SDRAMDISKFatFS; /* File system object for SDRAMDISK logical drive */
extern FIL SDRAMDISKFile; /* File object for SDRAMDISK */
extern uint8_t retSD; /* Return value for SD */
extern char SDPath[4]; /* SD logical drive path */
extern FATFS SDFatFS; /* File system object for SD logical drive */
extern FIL SDFile; /* File object for SD */

void MX_FATFS_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */
#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */
