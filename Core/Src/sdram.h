#ifndef _SDRAM_H_

#define _SDRAM_H_


#include "fmc.h"


#define SDRAM_MODEREG_BURST_LENGTH_1 ((uint16_t)0x0000)

#define SDRAM_MODEREG_BURST_LENGTH_2 ((uint16_t)0x0001)

#define SDRAM_MODEREG_BURST_LENGTH_4 ((uint16_t)0x0002)

#define SDRAM_MODEREG_BURST_LENGTH_8 ((uint16_t)0x0004)

#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL ((uint16_t)0x0000)

#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED ((uint16_t)0x0008)

#define SDRAM_MODEREG_CAS_LATENCY_2 ((uint16_t)0x0020)

#define SDRAM_MODEREG_CAS_LATENCY_3 ((uint16_t)0x0030)

#define SDRAM_MODEREG_OPERATING_MODE_STANDARD ((uint16_t)0x0000)

#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)

#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE ((uint16_t)0x0200)


void SDRAM_Init(void);

// // gary's solution
// uint8_t sdram_send_cmd(uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval);
// void sdram_initialization_sequence(void);

#endif /* _SDRAM_H_ */
