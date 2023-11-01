/**
 ****************************************************************************************************
 * @file        atk_rgblcd_touch_gtxx.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-24
 * @brief       ����ԭ��RGB LCDģ�鴥���������루GTXX��
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

// #include "./SYSTEM/delay/delay.h"
#include "atk_rgblcd_touch.h"
#include "atk_rgblcd_touch_iic.h"
#include "gpio.h"

#include <string.h>


#if (ATK_RGBLCD_USING_TOUCH != 0)

	/* RGB LCDģ�鴥����PID */
	#define ATK_RGBLCD_TOUCH_PID  "911"
	#define ATK_RGBLCD_TOUCH_PID1 "9147"
	#define ATK_RGBLCD_TOUCH_PID2 "9271"
	#define ATK_RGBLCD_TOUCH_PID3 "1158"

	/* RGB LCDģ������������� */
	#define ATK_RGBLCD_TOUCH_TP_MAX 5

	/* RGB LCDģ�鴥�����ּĴ������� */
	#define ATK_RGBLCD_TOUCH_REG_CTRL	0x8040 /* ���ƼĴ��� */
	#define ATK_RGBLCD_TOUCH_REG_PID	0x8140 /* PID�Ĵ��� */
	#define ATK_RGBLCD_TOUCH_REG_TPINFO 0x814E /* ����״̬�Ĵ��� */
	#define ATK_RGBLCD_TOUCH_REG_TP1	0x8150 /* ������1���ݼĴ��� */
	#define ATK_RGBLCD_TOUCH_REG_TP2	0x8158 /* ������2���ݼĴ��� */
	#define ATK_RGBLCD_TOUCH_REG_TP3	0x8160 /* ������3���ݼĴ��� */
	#define ATK_RGBLCD_TOUCH_REG_TP4	0x8168 /* ������4���ݼĴ��� */
	#define ATK_RGBLCD_TOUCH_REG_TP5	0x8170 /* ������5���ݼĴ��� */

	/* ����״̬�Ĵ������� */
	#define ATK_RGBLCD_TOUCH_TPINFO_MASK_STA 0x80
	#define ATK_RGBLCD_TOUCH_TPINFO_MASK_CNT 0x0F

/* RGB LCDģ�鴥�������ݼĴ��� */
static const uint16_t g_atk_rgblcd_touch_tp_reg[ATK_RGBLCD_TOUCH_TP_MAX] = {
	ATK_RGBLCD_TOUCH_REG_TP1, ATK_RGBLCD_TOUCH_REG_TP2, ATK_RGBLCD_TOUCH_REG_TP3,
	ATK_RGBLCD_TOUCH_REG_TP4, ATK_RGBLCD_TOUCH_REG_TP5,
};

/* RGB LCDģ�鴥��״̬�ṹ�� */
static struct {
	atk_rgblcd_touch_iic_addr_t iic_addr;
} g_atk_rgblcd_touch_sta;

/**
 * @brief       RGB LCDģ�鴥��Ӳ����ʼ��
 * @param       ��
 * @retval      ��
 */
static void atk_rgblcd_touch_hw_init(void) {
	GPIO_InitTypeDef gpio_init_struct = {0};

	/* ʹ��ʱ�� */
	ATK_RGBLCD_TOUCH_PEN_GPIO_CLK_ENABLE();
	ATK_RGBLCD_TOUCH_TCS_GPIO_CLK_ENABLE();

	/* ��ʼ��PEN���� */
	gpio_init_struct.Pin = ATK_RGBLCD_TOUCH_PEN_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;
	gpio_init_struct.Pull = GPIO_NOPULL;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

	/* ��ʼ��TCS���� */
	gpio_init_struct.Pin = ATK_RGBLCD_TOUCH_TCS_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Pull = GPIO_PULLUP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ATK_RGBLCD_TOUCH_TCS_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief       RGB LCDģ�鴥��Ӳ����λ
 * @param       addr: ��λ��ʹ�õ�IICͨѶ��ַ
 * @retval      ��
 */
static void atk_rgblcd_touch_hw_reset(atk_rgblcd_touch_iic_addr_t addr) {
	GPIO_InitTypeDef gpio_init_struct = {0};

	/* ����PEN����Ϊ��� */
	gpio_init_struct.Pin = ATK_RGBLCD_TOUCH_PEN_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Pull = GPIO_PULLUP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

	switch (addr) {
		case ATK_RGBLCD_TOUCH_IIC_ADDR_14: {
			ATK_RGBLCD_TOUCH_TCS(0);
			ATK_RGBLCD_TOUCH_PEN(0);
			ATK_RGBLCD_TOUCH_PEN(1);
			HAL_Delay(1);
			ATK_RGBLCD_TOUCH_TCS(1);
			HAL_Delay(10);
			break;
		}
		case ATK_RGBLCD_TOUCH_IIC_ADDR_5D: {
			ATK_RGBLCD_TOUCH_TCS(0);
			ATK_RGBLCD_TOUCH_PEN(0);
			HAL_Delay(1);
			ATK_RGBLCD_TOUCH_TCS(1);
			HAL_Delay(10);
			break;
		}
		default: {
			break;
		}
	}

	/* ��������PEN����Ϊ���� */
	gpio_init_struct.Pin = ATK_RGBLCD_TOUCH_PEN_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;
	gpio_init_struct.Pull = GPIO_NOPULL;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

	g_atk_rgblcd_touch_sta.iic_addr = addr;
	HAL_Delay(100);
}

/**
 * @brief       дRGB LCDģ�鴥���Ĵ���
 * @param       reg: ��д�Ĵ�����ַ
 *              buf: ��д�������
 *              len: ��д�����ݵĳ���
 * @retval      ATK_RGBLCD_TOUCH_EOK  : дATK-RGBLCDģ�鴥���Ĵ����ɹ�
 *              ATK_RGBLCD_TOUCH_ERROR: дATK-RGBLCDģ�鴥���Ĵ���ʧ��
 */
static uint8_t atk_rgblcd_touch_write_reg(uint16_t reg, uint8_t* buf, uint8_t len) {
	uint8_t buf_index;
	uint8_t ret;

	atk_rgblcd_touch_iic_start();
	atk_rgblcd_touch_iic_send_byte((g_atk_rgblcd_touch_sta.iic_addr << 1) | ATK_RGBLCD_TOUCH_IIC_WRITE);
	atk_rgblcd_touch_iic_wait_ack();
	atk_rgblcd_touch_iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
	atk_rgblcd_touch_iic_wait_ack();
	atk_rgblcd_touch_iic_send_byte((uint8_t)reg & 0xFF);
	atk_rgblcd_touch_iic_wait_ack();

	for (buf_index = 0; buf_index < len; buf_index++) {
		atk_rgblcd_touch_iic_send_byte(buf[buf_index]);
		ret = atk_rgblcd_touch_iic_wait_ack();
		if (ret != 0) {
			break;
		}
	}

	atk_rgblcd_touch_iic_stop();

	if (ret != 0) {
		return ATK_RGBLCD_TOUCH_ERROR;
	}

	return ATK_RGBLCD_TOUCH_EOK;
}

/**
 * @brief       ��RGB LCDģ�鴥���Ĵ���
 * @param       reg: �����Ĵ�����ַ
 *              buf: ��ȡ������
 *              len: ����ȡ���ݵĳ���
 * @retval      ��
 */
static void atk_rgblcd_touch_read_reg(uint16_t reg, uint8_t* buf, uint8_t len) {
	uint8_t buf_index;

	atk_rgblcd_touch_iic_start();
	atk_rgblcd_touch_iic_send_byte((g_atk_rgblcd_touch_sta.iic_addr << 1) | ATK_RGBLCD_TOUCH_IIC_WRITE);
	atk_rgblcd_touch_iic_wait_ack();
	atk_rgblcd_touch_iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
	atk_rgblcd_touch_iic_wait_ack();
	atk_rgblcd_touch_iic_send_byte((uint8_t)reg & 0xFF);
	atk_rgblcd_touch_iic_wait_ack();
	atk_rgblcd_touch_iic_start();
	atk_rgblcd_touch_iic_send_byte((g_atk_rgblcd_touch_sta.iic_addr << 1) | ATK_RGBLCD_TOUCH_IIC_READ);
	atk_rgblcd_touch_iic_wait_ack();

	for (buf_index = 0; buf_index < len - 1; buf_index++) {
		buf[buf_index] = atk_rgblcd_touch_iic_recv_byte(1);
	}

	buf[buf_index] = atk_rgblcd_touch_iic_recv_byte(0);

	atk_rgblcd_touch_iic_stop();
}

/**
 * @brief       RGB LCDģ�鴥��������λ
 * @param       ��
 * @retval      ��
 */
static void atk_rgblcd_touch_sw_reset(void) {
	uint8_t dat;

	dat = 0x02;
	atk_rgblcd_touch_write_reg(ATK_RGBLCD_TOUCH_REG_CTRL, &dat, 1);
	HAL_Delay(10);

	dat = 0x00;
	atk_rgblcd_touch_write_reg(ATK_RGBLCD_TOUCH_REG_CTRL, &dat, 1);
}

/**
 * @brief       ��ȡRGB LCDģ�鴥����PID
 * @param       pid: ��ȡ����PID��ASCII��
 * @retval      ��
 */
static void atk_rgblcd_touch_get_pid(char* pid) {
	atk_rgblcd_touch_read_reg(ATK_RGBLCD_TOUCH_REG_PID, (uint8_t*)pid, 4);
	pid[4] = '\0';
}

/**
 * @brief       RGB LCDģ�鴥����ʼ��
 * @param       ��
 * @retval      ATK_RGBLCD_TOUCH_EOK  : RGB LCDģ�鴥����ʼ���ɹ�
 *              ATK_RGBLCD_TOUCH_ERROR: RGB LCDģ�鴥����ʼ��ʧ��
 */
uint8_t atk_rgblcd_touch_init(atk_rgblcd_touch_type_t type) {
	char pid[5];

	if (type != ATK_RGBLCD_TOUCH_TYPE_GTXX) {
		return ATK_RGBLCD_TOUCH_ERROR;
	}

	atk_rgblcd_touch_hw_init();
	atk_rgblcd_touch_hw_reset(ATK_RGBLCD_TOUCH_IIC_ADDR_14);
	atk_rgblcd_touch_iic_init();
	atk_rgblcd_touch_get_pid(pid);
	if ((strcmp(pid, ATK_RGBLCD_TOUCH_PID) != 0) && (strcmp(pid, ATK_RGBLCD_TOUCH_PID1) != 0)
		&& (strcmp(pid, ATK_RGBLCD_TOUCH_PID2) != 0) && (strcmp(pid, ATK_RGBLCD_TOUCH_PID3) != 0)) {
		return ATK_RGBLCD_TOUCH_ERROR;
	}
	atk_rgblcd_touch_sw_reset();

	return ATK_RGBLCD_TOUCH_EOK;
}

/**
 * @brief       RGB LCDģ�鴥��ɨ��
 * @note        �������ü�������10ms
 * @param       point: ɨ�赽�Ĵ�������Ϣ
 *              cnt  : ��Ҫɨ��Ĵ�����������1~ATK_RGBLCD_TOUCH_TP_MAX��
 * @retval      0   : û��ɨ�赽������
 *              ����: ʵ�ʻ�ȡ���Ĵ�������Ϣ����
 */
uint8_t atk_rgblcd_touch_scan(atk_rgblcd_touch_point_t* point, uint8_t cnt) {
	uint8_t tp_info;
	uint8_t tp_cnt;
	uint8_t point_index;
	atk_rgblcd_lcd_disp_dir_t dir;
	uint8_t tpn_info[6];
	atk_rgblcd_touch_point_t point_raw;

	if ((cnt == 0) || (cnt > ATK_RGBLCD_TOUCH_TP_MAX)) {
		return 0;
	}

	for (point_index = 0; point_index < cnt; point_index++) {
		if (&point[point_index] == NULL) {
			return 0;
		}
	}

	atk_rgblcd_touch_read_reg(ATK_RGBLCD_TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));
	if ((tp_info & ATK_RGBLCD_TOUCH_TPINFO_MASK_STA) == ATK_RGBLCD_TOUCH_TPINFO_MASK_STA) {
		tp_cnt = tp_info & ATK_RGBLCD_TOUCH_TPINFO_MASK_CNT;
		tp_cnt = (cnt < tp_cnt) ? cnt : tp_cnt;

		for (point_index = 0; point_index < tp_cnt; point_index++) {
			atk_rgblcd_touch_read_reg(g_atk_rgblcd_touch_tp_reg[point_index], tpn_info, sizeof(tpn_info));
			point_raw.x = (uint16_t)(tpn_info[1] << 8) | tpn_info[0];
			point_raw.y = (uint16_t)(tpn_info[3] << 8) | tpn_info[2];
			point_raw.size = (uint16_t)(tpn_info[5] << 8) | tpn_info[4];

			dir = atk_rgblcd_get_disp_dir();
			switch (dir) {
				case ATK_RGBLCD_LCD_DISP_DIR_0: {
					point[point_index].x = point_raw.x;
					point[point_index].y = point_raw.y;
					break;
				}
				case ATK_RGBLCD_LCD_DISP_DIR_90: {
					point[point_index].x = point_raw.y;
					point[point_index].y = atk_rgblcd_get_lcd_height() - point_raw.x;
					break;
				}
				case ATK_RGBLCD_LCD_DISP_DIR_180: {
					point[point_index].x = atk_rgblcd_get_lcd_width() - point_raw.x;
					point[point_index].y = atk_rgblcd_get_lcd_height() - point_raw.y;
					break;
				}
				case ATK_RGBLCD_LCD_DISP_DIR_270: {
					point[point_index].x = atk_rgblcd_get_lcd_width() - point_raw.y;
					point[point_index].y = point_raw.x;
					break;
				}
			}

			point[point_index].size = point_raw.size;
		}

		tp_info = 0;
		atk_rgblcd_touch_write_reg(ATK_RGBLCD_TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));

		return tp_cnt;
	} else {
		return 0;
	}
}

#endif
