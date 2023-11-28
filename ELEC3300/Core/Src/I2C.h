#ifndef __I2C_H
#define __I2C_H


/**
 * @brief  ����I2C�ӿ�.
 * @note
 * @retval None
 */
void I2C_Init(void);


/**
 * @brief  I2C�ӿڸ�λ
 * @note
 * @retval None
 */
void I2C_DeInit(void);


/**
 * @brief ����������ʼ�ź�
 * @retval
 */
void I2C_Start(void);


/**
 * @brief  ��������ֹͣ�ź�
 * @retval
 */
void I2C_Stop(void);


/**
 * @brief  ��������Ӧ���ź�
 * @retval
 */
void I2C_SendACK(void);


/**
 * @brief  �������ͷ�Ӧ���ź�
 * @retval
 */
void I2C_SendNACK(void);


/**
 * @brief  ����Ƿ���յ�Ӧ���ź�
 * @retval
 */
unsigned char Check_ACK(void);


/**
 * @brief I2C����һ���ֽ�
 * @param byte: ���͵�����
 * @retval
 */
void I2C_SendByte(unsigned char byte);


/**
 * @brief I2C����һ���ֽ�
 * @retval ���ؽ��յ�������
 */
unsigned char I2C_ReciveByte(void);


#endif
