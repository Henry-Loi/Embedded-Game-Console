#pragma once

#include "delay.h"
#include "gpio.h"

#define SIIC_SDA_PORT	  GPIOI
#define SIIC_SDA_PIN	  GPIO_PIN_3
#define SIIC_SDA_PORT_LCK __HAL_RCC_GPIOI_CLK_ENABLE(); // 使能GPIOH时钟


#define SIIC_SCL_PORT	  GPIOH
#define SIIC_SCL_PIN	  GPIO_PIN_6
#define SIIC_SCL_PORT_LCK __HAL_RCC_GPIOH_CLK_ENABLE(); // 使能GPIOH时钟


#define SIIC_SDA_H HAL_GPIO_WritePin(SIIC_SDA_PORT, SIIC_SDA_PIN, GPIO_PIN_SET)
#define SIIC_SDA_L HAL_GPIO_WritePin(SIIC_SDA_PORT, SIIC_SDA_PIN, GPIO_PIN_RESET)

#define SIIC_SCL_H HAL_GPIO_WritePin(SIIC_SCL_PORT, SIIC_SCL_PIN, GPIO_PIN_SET)
#define SIIC_SCL_L HAL_GPIO_WritePin(SIIC_SCL_PORT, SIIC_SCL_PIN, GPIO_PIN_RESET)

#define SIIC_SPEED_DELAY()		 delay_us(2)
#define SIIC_SPEED_START_DELAY() delay_us(30)


void SIIC_Init(void);
void SIIC_SDA_OUT();
void SIIC_SDA_IN();
void SIIC_Start(void);
void SIIC_Stop(void);
void SIIC_Send_Byte(uint8_t txd);
uint8_t SIIC_Read_Byte(unsigned char ack);
uint8_t SIIC_Wait_Ack(void);
void SIIC_Ack(void);
void SIIC_NAck(void);
void SIIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
uint8_t SIIC_Read_One_Byte(uint8_t daddr, uint8_t addr);
