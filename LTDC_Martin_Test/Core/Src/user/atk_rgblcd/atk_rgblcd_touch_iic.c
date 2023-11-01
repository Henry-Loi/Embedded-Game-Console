/**
 ****************************************************************************************************
 * @file        atk_rgblcd_touch_iic.c
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

#include "atk_rgblcd_touch_iic.h"

#include "gpio.h"


// #include "./SYSTEM/delay/delay.h"


#if (ATK_RGBLCD_USING_TOUCH != 0)

/**
 * @brief       IIC�ӿ���ʱ���������ڿ���IIC��д�ٶ�
 * @param       ��
 * @retval      ��
 */
static inline void atk_rgblcd_touch_iic_delay(void) { HAL_Delay(1); }

/**
 * @brief       ��ʼ��IIC�ӿ�
 * @param       ��
 * @retval      ��
 */
void atk_rgblcd_touch_iic_init(void) {
	GPIO_InitTypeDef gpio_init_struct = {0};

	/* ʹ��SCL��SDA����GPIO��ʱ�� */
	ATK_RGBLCD_TOUCH_IIC_SCL_GPIO_CLK_ENABLE();
	ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_CLK_ENABLE();

	/* ��ʼ��SCL���� */
	gpio_init_struct.Pin = ATK_RGBLCD_TOUCH_IIC_SCL_GPIO_PIN; /* SCL���� */
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;			  /* ������� */
	gpio_init_struct.Pull = GPIO_PULLUP;					  /* ���� */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;			  /* ���� */
	HAL_GPIO_Init(ATK_RGBLCD_TOUCH_IIC_SCL_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��SDA���� */
	gpio_init_struct.Pin = ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_PIN; /* SDA���� */
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;			  /* ��©��� */
	HAL_GPIO_Init(ATK_RGBLCD_TOUCH_IIC_SDA_GPIO_PORT, &gpio_init_struct);

	atk_rgblcd_touch_iic_stop();
}

/**
 * @brief       ����IIC��ʼ�ź�
 * @param       ��
 * @retval      ��
 */
void atk_rgblcd_touch_iic_start(void) {
	ATK_RGBLCD_TOUCH_IIC_SDA(1);
	ATK_RGBLCD_TOUCH_IIC_SCL(1);
	atk_rgblcd_touch_iic_delay();
	ATK_RGBLCD_TOUCH_IIC_SDA(0);
	atk_rgblcd_touch_iic_delay();
	ATK_RGBLCD_TOUCH_IIC_SCL(0);
	atk_rgblcd_touch_iic_delay();
}

/**
 * @brief       ����IICֹͣ�ź�
 * @param       ��
 * @retval      ��
 */
void atk_rgblcd_touch_iic_stop(void) {
	ATK_RGBLCD_TOUCH_IIC_SDA(0);
	atk_rgblcd_touch_iic_delay();
	ATK_RGBLCD_TOUCH_IIC_SCL(1);
	atk_rgblcd_touch_iic_delay();
	ATK_RGBLCD_TOUCH_IIC_SDA(1);
	atk_rgblcd_touch_iic_delay();
}

/**
 * @brief       �ȴ�IICӦ���ź�
 * @param       ��
 * @retval      0: Ӧ���źŽ��ճɹ�
 *              1: Ӧ���źŽ���ʧ��
 */
uint8_t atk_rgblcd_touch_iic_wait_ack(void) {
	uint8_t waittime = 0;
	uint8_t rack = 0;

	ATK_RGBLCD_TOUCH_IIC_SDA(1);
	atk_rgblcd_touch_iic_delay();
	ATK_RGBLCD_TOUCH_IIC_SCL(1);
	atk_rgblcd_touch_iic_delay();

	while (ATK_RGBLCD_TOUCH_IIC_READ_SDA()) {
		waittime++;

		if (waittime > 250) {
			atk_rgblcd_touch_iic_stop();
			rack = 1;
			break;
		}
	}

	ATK_RGBLCD_TOUCH_IIC_SCL(0);
	atk_rgblcd_touch_iic_delay();

	return rack;
}

/**
 * @brief       ����ACKӦ���ź�
 * @param       ��
 * @retval      ��
 */
void atk_rgblcd_touch_iic_ack(void) {
	ATK_RGBLCD_TOUCH_IIC_SDA(0);
	atk_rgblcd_touch_iic_delay();
	ATK_RGBLCD_TOUCH_IIC_SCL(1);
	atk_rgblcd_touch_iic_delay();
	ATK_RGBLCD_TOUCH_IIC_SCL(0);
	atk_rgblcd_touch_iic_delay();
	ATK_RGBLCD_TOUCH_IIC_SDA(1);
	atk_rgblcd_touch_iic_delay();
}

/**
 * @brief       ������ACKӦ���ź�
 * @param       ��
 * @retval      ��
 */
void atk_rgblcd_touch_iic_nack(void) {
	ATK_RGBLCD_TOUCH_IIC_SDA(1);
	atk_rgblcd_touch_iic_delay();
	ATK_RGBLCD_TOUCH_IIC_SCL(1);
	atk_rgblcd_touch_iic_delay();
	ATK_RGBLCD_TOUCH_IIC_SCL(0);
	atk_rgblcd_touch_iic_delay();
}

/**
 * @brief       IIC����һ���ֽ�
 * @param       dat: Ҫ���͵�����
 * @retval      ��
 */
void atk_rgblcd_touch_iic_send_byte(uint8_t dat) {
	uint8_t t;

	for (t = 0; t < 8; t++) {
		ATK_RGBLCD_TOUCH_IIC_SDA((dat & 0x80) >> 7);
		atk_rgblcd_touch_iic_delay();
		ATK_RGBLCD_TOUCH_IIC_SCL(1);
		atk_rgblcd_touch_iic_delay();
		ATK_RGBLCD_TOUCH_IIC_SCL(0);
		dat <<= 1;
	}
	ATK_RGBLCD_TOUCH_IIC_SDA(1);
}

/**
 * @brief       IIC����һ���ֽ�
 * @param       ack: ack=1ʱ������ack; ack=0ʱ������nack
 * @retval      ���յ�������
 */
uint8_t atk_rgblcd_touch_iic_recv_byte(uint8_t ack) {
	uint8_t i;
	uint8_t dat = 0;

	for (i = 0; i < 8; i++) {
		dat <<= 1;
		ATK_RGBLCD_TOUCH_IIC_SCL(1);
		atk_rgblcd_touch_iic_delay();

		if (ATK_RGBLCD_TOUCH_IIC_READ_SDA()) {
			dat++;
		}

		ATK_RGBLCD_TOUCH_IIC_SCL(0);
		atk_rgblcd_touch_iic_delay();
	}

	if (ack == 0) {
		atk_rgblcd_touch_iic_nack();
	} else {
		atk_rgblcd_touch_iic_ack();
	}

	return dat;
}

#endif
