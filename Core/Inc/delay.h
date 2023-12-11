#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"

void delay(uint32_t ms);
void delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DELAY_H */