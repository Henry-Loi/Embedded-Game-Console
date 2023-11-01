/**
 ****************************************************************************************************
 * @file        atk_rgblcd_touch.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-24
 * @brief       ����ԭ��RGB LCDģ�鴥����������
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

#ifndef __ATK_RGBLCD_TOUCH_H
#define __ATK_RGBLCD_TOUCH_H

#include "atk_rgblcd.h"

#if (ATK_RGBLCD_USING_TOUCH != 0)

	/* ���Ŷ��� */
	#define ATK_RGBLCD_TOUCH_PEN_GPIO_PORT GPIOH
	#define ATK_RGBLCD_TOUCH_PEN_GPIO_PIN  GPIO_PIN_7
	#define ATK_RGBLCD_TOUCH_PEN_GPIO_CLK_ENABLE() \
		do {                                       \
			__HAL_RCC_GPIOH_CLK_ENABLE();          \
		} while (0)
	#define ATK_RGBLCD_TOUCH_TCS_GPIO_PORT GPIOI
	#define ATK_RGBLCD_TOUCH_TCS_GPIO_PIN  GPIO_PIN_8
	#define ATK_RGBLCD_TOUCH_TCS_GPIO_CLK_ENABLE() \
		do {                                       \
			__HAL_RCC_GPIOI_CLK_ENABLE();          \
		} while (0)

	/* IO���� */
	#define ATK_RGBLCD_TOUCH_READ_PEN() HAL_GPIO_ReadPin(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, ATK_RGBLCD_TOUCH_PEN_GPIO_PIN)
	#define ATK_RGBLCD_TOUCH_PEN(x)                                                                               \
		do {                                                                                                      \
			x ? HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, ATK_RGBLCD_TOUCH_PEN_GPIO_PIN, GPIO_PIN_SET)    \
			  : HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, ATK_RGBLCD_TOUCH_PEN_GPIO_PIN, GPIO_PIN_RESET); \
		} while (0)
	#define ATK_RGBLCD_TOUCH_TCS(x)                                                                               \
		do {                                                                                                      \
			x ? HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_TCS_GPIO_PORT, ATK_RGBLCD_TOUCH_TCS_GPIO_PIN, GPIO_PIN_SET)    \
			  : HAL_GPIO_WritePin(ATK_RGBLCD_TOUCH_TCS_GPIO_PORT, ATK_RGBLCD_TOUCH_TCS_GPIO_PIN, GPIO_PIN_RESET); \
		} while (0)

/* RGB LCD����IICͨѶ��ַö�� */
typedef enum {
	ATK_RGBLCD_TOUCH_IIC_ADDR_14 = 0x14, /* 0x14 */
	ATK_RGBLCD_TOUCH_IIC_ADDR_5D = 0x5D, /* 0x5D */
	ATK_RGBLCD_TOUCH_IIC_ADDR_38 = 0x38, /* 0x38 */
} atk_rgblcd_touch_iic_addr_t;

/* �������������ݽṹ */
typedef struct {
	uint16_t x;	   /* ������X���� */
	uint16_t y;	   /* ������Y���� */
	uint16_t size; /* �������С */
} atk_rgblcd_touch_point_t;

	/* ������� */
	#define ATK_RGBLCD_TOUCH_EOK   0 /* û�д��� */
	#define ATK_RGBLCD_TOUCH_ERROR 1 /* ���� */

/* �������� */
uint8_t atk_rgblcd_touch_init(atk_rgblcd_touch_type_t type);				 /* RGB LCDģ�鴥����ʼ�� */
uint8_t atk_rgblcd_touch_scan(atk_rgblcd_touch_point_t* point, uint8_t cnt); /* RGB LCDģ�鴥��ɨ�� */

#endif

#endif
