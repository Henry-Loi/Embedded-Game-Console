/**
 ****************************************************************************************************
 * @file        atk_rgblcd_touch_iic.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-24
 * @brief       ����ԭ��RGB LCDģ�鴥��IIC�ӿ���������
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ F429������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __ATK_RGBLCD_TOUCH_IIC_H
#define __ATK_RGBLCD_TOUCH_IIC_H

#include "atk_rgblcd.h"

#if (ATK_RGBLCD_USING_TOUCH != 0)

	/* ���Ŷ��� */
	#define ATK_RGBLCD_TOUCH_IIC_SCL_GPIO_PORT GPIOH
	#define ATK_RGBLCD_TOUCH_IIC_SCL_GPIO_PIN  GPIO_PIN_6
	#define ATK_RGBLCD_TOUCH_IIC_SCL_GPIO_CLK_ENABLE() \
		do {                                           \
			__HAL_RCC_GPIOH_CLK_ENABLE();              \
		} while (0)
	#define ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_PORT GPIOI
	#define ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_PIN  GPIO_PIN_3
	#define ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_CLK_ENABLE() \
		do {                                           \
			__HAL_RCC_GPIOI_CLK_ENABLE();              \
		} while (0)

	/* IO���� */
	#define ATK_RGBLCD_TOUCH_IIC_SCL(x)                                                                                \
		do {                                                                                                           \
			x ? HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_IIC_SCL_GPIO_PORT, ATK_RGBLCD_TOUCH_IIC_SCL_GPIO_PIN, GPIO_PIN_SET) \
			  : HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_IIC_SCL_GPIO_PORT, ATK_RGBLCD_TOUCH_IIC_SCL_GPIO_PIN,               \
								  GPIO_PIN_RESET);                                                                     \
		} while (0)

	#define ATK_RGBLCD_TOUCH_IIC_SDA(x)                                                                                \
		do {                                                                                                           \
			x ? HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_PORT, ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_PIN, GPIO_PIN_SET) \
			  : HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_PORT, ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_PIN,               \
								  GPIO_PIN_RESET);                                                                     \
		} while (0)

	#define ATK_RGBLCD_TOUCH_IIC_READ_SDA() \
		HAL_GPIO_ReadPin(ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_PORT, ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_PIN)

	/* ����IICͨѶ�Ƕ���������д���� */
	#define ATK_RGBLCD_TOUCH_IIC_WRITE 0
	#define ATK_RGBLCD_TOUCH_IIC_READ  1

	/* ������� */
	#define ATK_RGBLCD_TOUCH_IIC_EOK   0 /* û�д��� */
	#define ATK_RGBLCD_TOUCH_IIC_ERROR 1 /* ���� */

/* �������� */
void atk_rgblcd_touch_iic_init(void);				 /* ��ʼ��IIC�ӿ� */
void atk_rgblcd_touch_iic_start(void);				 /* ����IIC��ʼ�ź� */
void atk_rgblcd_touch_iic_stop(void);				 /* ����IICֹͣ�ź� */
uint8_t atk_rgblcd_touch_iic_wait_ack(void);		 /* �ȴ�IICӦ���ź� */
void atk_rgblcd_touch_iic_ack(void);				 /* ����ACKӦ���ź� */
void atk_rgblcd_touch_iic_nack(void);				 /* ������ACKӦ���ź� */
void atk_rgblcd_touch_iic_send_byte(uint8_t dat);	 /* IIC����һ���ֽ� */
uint8_t atk_rgblcd_touch_iic_recv_byte(uint8_t ack); /* IIC����һ���ֽ� */

#endif

#endif
