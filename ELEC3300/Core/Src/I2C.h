#ifndef __I2C_H
#define __I2C_H


/**
 * @brief  配置I2C接口.
 * @note
 * @retval None
 */
void I2C_Init(void);


/**
 * @brief  I2C接口复位
 * @note
 * @retval None
 */
void I2C_DeInit(void);


/**
 * @brief 主机产生起始信号
 * @retval
 */
void I2C_Start(void);


/**
 * @brief  主机产生停止信号
 * @retval
 */
void I2C_Stop(void);


/**
 * @brief  主机发送应答信号
 * @retval
 */
void I2C_SendACK(void);


/**
 * @brief  主机发送非应答信号
 * @retval
 */
void I2C_SendNACK(void);


/**
 * @brief  检查是否接收到应答信号
 * @retval
 */
unsigned char Check_ACK(void);


/**
 * @brief I2C发送一个字节
 * @param byte: 发送的数据
 * @retval
 */
void I2C_SendByte(unsigned char byte);


/**
 * @brief I2C接收一个字节
 * @retval 返回接收到的数据
 */
unsigned char I2C_ReciveByte(void);


#endif
