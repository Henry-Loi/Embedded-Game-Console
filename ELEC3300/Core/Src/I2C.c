#include "I2C.h"

#include "stm32f4xx_hal.h"


#define SCL_LOW	  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_RESET)
#define SCL_HIGHT HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_SET)

#define SDA_LOW	  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_RESET)
#define SDA_HIGHT HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_SET)

#define SDA_READ() HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_5)


/**
 * @brief  I2C的延时函数，延时xUS
 * @note
 * @retval None
 */
void I2C_Delay_1us(unsigned int us) {
	unsigned int i, j;

	for (i = 0; i < us; i++)
		for (j = 0; j < 40; j++)
			;
}


/**
 * @brief  配置I2C接口.
 * @note
 * @retval None
 */
void I2C_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOH_CLK_ENABLE();

	HAL_GPIO_DeInit(GPIOH, (GPIO_PIN_4 | GPIO_PIN_5));
	I2C_Delay_1us(10000);

	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	I2C_Stop();
	I2C_Delay_1us(10000);
}


/**
 * @brief  I2C接口复位
 * @note
 * @retval None
 */
void I2C_DeInit(void) { HAL_GPIO_DeInit(GPIOH, (GPIO_PIN_4 | GPIO_PIN_5)); }


/*
**
* @brief  配置SDA引脚
* @note
* @param mode: 1--输出    0--输入
* @retval None
*/
void SDA_GPIO_Config(unsigned char mode) {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_5;

	if (mode == 1) {
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	} else {
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	}
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}


/**
 * @brief 主机产生起始信号
 * @retval
 */
void I2C_Start(void) {
	SDA_HIGHT;
	SCL_HIGHT;

	I2C_Delay_1us(3);

	SDA_LOW;

	I2C_Delay_1us(2);

	SCL_LOW;
}


/**
 * @brief  主机产生停止信号
 * @retval
 */
void I2C_Stop(void) {
	SDA_LOW;
	SCL_HIGHT;

	I2C_Delay_1us(3);

	SDA_HIGHT;

	I2C_Delay_1us(2);
}


/**
 * @brief  主机发送应答信号
 * @retval
 */
void I2C_SendACK(void) {
	SCL_LOW;
	SDA_LOW;

	I2C_Delay_1us(5);

	SCL_HIGHT;

	I2C_Delay_1us(5);

	SCL_LOW;
}


/**
 * @brief  主机发送非应答信号
 * @retval
 */
void I2C_SendNACK(void) {
	SCL_LOW;
	SDA_HIGHT;

	I2C_Delay_1us(5);

	SCL_HIGHT;

	I2C_Delay_1us(5);

	SCL_LOW;
}


/**
 * @brief  检查是否接收到应答信号
 * @retval
 */
unsigned char Check_ACK(void) {
	SCL_LOW;
	SDA_HIGHT;

	I2C_Delay_1us(5);

	SDA_GPIO_Config(0);

	SCL_HIGHT;

	I2C_Delay_1us(5);

	if (SDA_READ() == 0X00) {
		SCL_LOW;
		SDA_GPIO_Config(1);
		return 1;
	}

	SCL_LOW;
	SDA_GPIO_Config(1);
	return 0;
}


/**
 * @brief I2C发送一个字节
 * @param byte: 发送的数据
 * @retval
 */
void I2C_SendByte(unsigned char byte) {
	unsigned char i, temp;

	temp = byte;

	for (i = 8; i > 0; i--) {
		SCL_LOW;

		if ((temp & 0x80) == 0x80) {
			SDA_HIGHT;
		} else
			SDA_LOW;

		temp = temp << 1;

		I2C_Delay_1us(5);

		SCL_HIGHT;

		I2C_Delay_1us(5);
	}

	SCL_LOW;
}


/**
 * @brief I2C接收一个字节
 * @retval 返回接收到的数据
 */
unsigned char I2C_ReciveByte(void) {
	unsigned char i, temp = 0;

	SDA_GPIO_Config(0);

	for (i = 8; i > 0; i--) {
		temp <<= 1;
		SCL_LOW;

		I2C_Delay_1us(5);

		SCL_HIGHT;

		I2C_Delay_1us(5);

		if (SDA_READ() == 0x01) {
			temp = temp | 0x01;
		}
	}

	SCL_LOW;
	SDA_GPIO_Config(1);

	return temp;
}
