// /**
//  ****************************************************************************************************
//  * @file        atk_rgblcd_touch_ftxx.c
//  * @author      ����ԭ���Ŷ�(ALIENTEK)
//  * @version     V1.0
//  * @date        2023-02-24
//  * @brief       ����ԭ��RGB LCDģ�鴥���������루FTXX��
//  * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
//  ****************************************************************************************************
//  * @attention
//  *
//  * ʵ��ƽ̨:����ԭ�� ������ F429������
//  * ������Ƶ:www.yuanzige.com
//  * ������̳:www.openedv.com
//  * ��˾��ַ:www.alientek.com
//  * �����ַ:openedv.taobao.com
//  *
//  ****************************************************************************************************
//  */

// // #include "./SYSTEM/delay/delay.h"
// #include "atk_rgblcd_touch.h"
// #include "atk_rgblcd_touch_iic.h"
// #include "gpio.h"

// #include "stm32f4xx_hal.h"

// #include <string.h>


// #if (ATK_RGBLCD_USING_TOUCH != 0)

// 	/* RGB LCDģ������������� */
// 	#define ATK_RGBLCD_TOUCH_TP_MAX 5

// 	/* RGB LCDģ�鴥�����ּĴ������� */
// 	#define ATK_RGBLCD_TOUCH_REG_DEVIDE_MODE	   0x00 /* �豸ģʽ���ƼĴ��� */
// 	#define ATK_RGBLCD_TOUCH_REG_ID_G_MODE		   0xA4 /* �ж�ģʽ���ƼĴ��� */
// 	#define ATK_RGBLCD_TOUCH_REG_ID_G_THGROUP	   0x80 /* ��Ч������ֵ���üĴ��� */
// 	#define ATK_RGBLCD_TOUCH_REG_ID_G_PERIODACTIVE 0x88 /* �����������üĴ��� */
// 	#define ATK_RGBLCD_TOUCH_REG_TD_STATUS		   0x02 /* ����״̬�Ĵ��� */
// 	#define ATK_RGBLCD_TOUCH_REG_TP1			   0x03 /* ������1���ݼĴ��� */
// 	#define ATK_RGBLCD_TOUCH_REG_TP2			   0x09 /* ������2���ݼĴ��� */
// 	#define ATK_RGBLCD_TOUCH_REG_TP3			   0x0F /* ������3���ݼĴ��� */
// 	#define ATK_RGBLCD_TOUCH_REG_TP4			   0x15 /* ������4���ݼĴ��� */
// 	#define ATK_RGBLCD_TOUCH_REG_TP5			   0x1B /* ������5���ݼĴ��� */

// 	/* ����״̬�Ĵ������� */
// 	#define ATK_RGBLCD_TOUCH_TD_STATUS_MASK_CNT 0x0F

// /* RGB LCDģ�鴥�������ݼĴ��� */
// static const uint16_t g_atk_rgblcd_touch_tp_reg[ATK_RGBLCD_TOUCH_TP_MAX] = {
// 	ATK_RGBLCD_TOUCH_REG_TP1, ATK_RGBLCD_TOUCH_REG_TP2, ATK_RGBLCD_TOUCH_REG_TP3,
// 	ATK_RGBLCD_TOUCH_REG_TP4, ATK_RGBLCD_TOUCH_REG_TP5,
// };

// /**
//  * @brief       RGB LCDģ�鴥��Ӳ����ʼ��
//  * @param       ��
//  * @retval      ��
//  */
// static void atk_rgblcd_touch_hw_init(void) {
// 	GPIO_InitTypeDef gpio_init_struct = {0};

// 	/* ʹ��ʱ�� */
// 	ATK_RGBLCD_TOUCH_PEN_GPIO_CLK_ENABLE();
// 	ATK_RGBLCD_TOUCH_TCS_GPIO_CLK_ENABLE();

// 	/* ��ʼ��PEN���� */
// 	gpio_init_struct.Pin = ATK_RGBLCD_TOUCH_PEN_GPIO_PIN;
// 	gpio_init_struct.Mode = GPIO_MODE_INPUT;
// 	gpio_init_struct.Pull = GPIO_PULLUP;
// 	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
// 	HAL_GPIO_Init(ATK_RGBLCD_TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

// 	/* ��ʼ��TCS���� */
// 	gpio_init_struct.Pin = ATK_RGBLCD_TOUCH_TCS_GPIO_PIN;
// 	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
// 	gpio_init_struct.Pull = GPIO_PULLUP;
// 	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
// 	HAL_GPIO_Init(ATK_RGBLCD_TOUCH_TCS_GPIO_PORT, &gpio_init_struct);
// }

// /**
//  * @brief       RGB LCDģ�鴥��Ӳ����λ
//  * @param       ��
//  * @retval      ��
//  */
// static void atk_rgblcd_touch_hw_reset(void) {
// 	ATK_RGBLCD_TOUCH_TCS(0);
// 	HAL_Delay(20);
// 	ATK_RGBLCD_TOUCH_TCS(1);
// 	HAL_Delay(50);
// }

// /**
//  * @brief       дRGB LCDģ�鴥���Ĵ���
//  * @param       reg: ��д�Ĵ�����ַ
//  *              buf: ��д�������
//  *              len: ��д�����ݵĳ���
//  * @retval      ATK_RGBLCD_TOUCH_EOK  : дATK-RGBLCDģ�鴥���Ĵ����ɹ�
//  *              ATK_RGBLCD_TOUCH_ERROR: дATK-RGBLCDģ�鴥���Ĵ���ʧ��
//  */
// static uint8_t atk_rgblcd_touch_write_reg(uint16_t reg, uint8_t* buf, uint8_t len) {
// 	uint8_t buf_index;
// 	uint8_t ret;

// 	atk_rgblcd_touch_iic_start();
// 	atk_rgblcd_touch_iic_send_byte((ATK_RGBLCD_TOUCH_IIC_ADDR_38 << 1) | ATK_RGBLCD_TOUCH_IIC_WRITE);
// 	atk_rgblcd_touch_iic_wait_ack();
// 	atk_rgblcd_touch_iic_send_byte(reg);
// 	atk_rgblcd_touch_iic_wait_ack();

// 	for (buf_index = 0; buf_index < len; buf_index++) {
// 		atk_rgblcd_touch_iic_send_byte(buf[buf_index]);
// 		ret = atk_rgblcd_touch_iic_wait_ack();
// 		if (ret != 0) {
// 			break;
// 		}
// 	}

// 	atk_rgblcd_touch_iic_stop();

// 	if (ret != 0) {
// 		return ATK_RGBLCD_TOUCH_ERROR;
// 	}

// 	return ATK_RGBLCD_TOUCH_EOK;
// }

// /**
//  * @brief       ��RGB LCDģ�鴥���Ĵ���
//  * @param       reg: �����Ĵ�����ַ
//  *              buf: ��ȡ������
//  *              len: ����ȡ���ݵĳ���
//  * @retval      ��
//  */
// void atk_rgblcd_touch_iic_read_reg(uint8_t reg, uint8_t* buf, uint8_t len) {
// 	uint8_t buf_index;

// 	atk_rgblcd_touch_iic_start();
// 	atk_rgblcd_touch_iic_send_byte((ATK_RGBLCD_TOUCH_IIC_ADDR_38 << 1) | ATK_RGBLCD_TOUCH_IIC_WRITE);
// 	atk_rgblcd_touch_iic_wait_ack();
// 	atk_rgblcd_touch_iic_send_byte(reg);
// 	atk_rgblcd_touch_iic_wait_ack();
// 	atk_rgblcd_touch_iic_start();
// 	atk_rgblcd_touch_iic_send_byte((ATK_RGBLCD_TOUCH_IIC_ADDR_38 << 1) | ATK_RGBLCD_TOUCH_IIC_READ);
// 	atk_rgblcd_touch_iic_wait_ack();

// 	for (buf_index = 0; buf_index < len - 1; buf_index++) {
// 		buf[buf_index] = atk_rgblcd_touch_iic_recv_byte(1);
// 	}

// 	buf[buf_index] = atk_rgblcd_touch_iic_recv_byte(0);

// 	atk_rgblcd_touch_iic_stop();
// }

// /**
//  * @brief       RGB LCDģ�鴥���Ĵ�����ʼ��
//  * @param       ��
//  * @retval      ��
//  */
// static void atk_rgblcd_touch_reg_init(void) {
// 	uint8_t dat;

// 	/* �豸ģʽ���ƼĴ��� */
// 	dat = 0x00;
// 	atk_rgblcd_touch_write_reg(ATK_RGBLCD_TOUCH_REG_DEVIDE_MODE, &dat, sizeof(dat));

// 	/* �ж�ģʽ���ƼĴ��� */
// 	dat = 0x00;
// 	atk_rgblcd_touch_write_reg(ATK_RGBLCD_TOUCH_REG_ID_G_MODE, &dat, sizeof(dat));

// 	/* ��Ч������ֵ���üĴ��� */
// 	dat = 22;
// 	atk_rgblcd_touch_write_reg(ATK_RGBLCD_TOUCH_REG_ID_G_THGROUP, &dat, sizeof(dat));

// 	/* �����������üĴ���������С��12 */
// 	dat = 12;
// 	atk_rgblcd_touch_write_reg(ATK_RGBLCD_TOUCH_REG_ID_G_PERIODACTIVE, &dat, sizeof(dat));
// }

// /**
//  * @brief       RGB LCDģ�鴥����ʼ��
//  * @param       ��
//  * @retval      ATK_RGBLCD_TOUCH_EOK  : RGB LCDģ�鴥����ʼ���ɹ�
//  *              ATK_RGBLCD_TOUCH_ERROR: RGB LCDģ�鴥����ʼ��ʧ��
//  */
// uint8_t atk_rgblcd_touch_init(atk_rgblcd_touch_type_t type) {
// 	if (type != ATK_RGBLCD_TOUCH_TYPE_FTXX) {
// 		return ATK_RGBLCD_TOUCH_ERROR;
// 	}

// 	atk_rgblcd_touch_hw_init();
// 	atk_rgblcd_touch_hw_reset();
// 	atk_rgblcd_touch_iic_init();
// 	atk_rgblcd_touch_reg_init();

// 	return ATK_RGBLCD_TOUCH_EOK;
// }

// /**
//  * @brief       RGB LCDģ�鴥��ɨ��
//  * @note        �������ü�������4ms
//  * @param       point: ɨ�赽�Ĵ�������Ϣ
//  *              cnt  : ��Ҫɨ��Ĵ�����������1~ATK_RGBLCD_TOUCH_TP_MAX��
//  * @retval      0   : û��ɨ�赽������
//  *              ����: ʵ�ʻ�ȡ���Ĵ�������Ϣ����
//  */
// uint8_t atk_rgblcd_touch_scan(atk_rgblcd_touch_point_t* point, uint8_t cnt) {
// 	uint8_t tp_stat;
// 	uint8_t tp_cnt;
// 	uint8_t point_index;
// 	atk_rgblcd_lcd_disp_dir_t dir;
// 	uint8_t tpn_info[4];
// 	atk_rgblcd_touch_point_t point_raw;

// 	if ((cnt == 0) || (cnt > ATK_RGBLCD_TOUCH_TP_MAX)) {
// 		return 0;
// 	}

// 	for (point_index = 0; point_index < cnt; point_index++) {
// 		if (&point[point_index] == NULL) {
// 			return 0;
// 		}
// 	}

// 	atk_rgblcd_touch_iic_read_reg(ATK_RGBLCD_TOUCH_REG_TD_STATUS, &tp_stat, sizeof(tp_stat));
// 	tp_cnt = tp_stat & ATK_RGBLCD_TOUCH_TD_STATUS_MASK_CNT;
// 	if ((tp_cnt != 0) && (tp_cnt <= ATK_RGBLCD_TOUCH_TP_MAX)) {
// 		tp_cnt = (cnt < tp_cnt) ? cnt : tp_cnt;

// 		for (point_index = 0; point_index < tp_cnt; point_index++) {
// 			atk_rgblcd_touch_iic_read_reg(g_atk_rgblcd_touch_tp_reg[point_index], tpn_info, sizeof(tpn_info));
// 			point_raw.x = (uint16_t)((tpn_info[0] & 0x0F) << 8) | tpn_info[1];
// 			point_raw.y = (uint16_t)((tpn_info[2] & 0x0F) << 8) | tpn_info[3];

// 			dir = atk_rgblcd_get_disp_dir();
// 			switch (dir) {
// 				case ATK_RGBLCD_LCD_DISP_DIR_0: {
// 					point[point_index].x = point_raw.y;
// 					point[point_index].y = point_raw.x;
// 					break;
// 				}
// 				case ATK_RGBLCD_LCD_DISP_DIR_90: {
// 					point[point_index].x = point_raw.x;
// 					point[point_index].y = atk_rgblcd_get_lcd_height() - point_raw.y;
// 					break;
// 				}
// 				case ATK_RGBLCD_LCD_DISP_DIR_180: {
// 					point[point_index].x = atk_rgblcd_get_lcd_width() - point_raw.y;
// 					point[point_index].y = atk_rgblcd_get_lcd_height() - point_raw.x;
// 					break;
// 				}
// 				case ATK_RGBLCD_LCD_DISP_DIR_270: {
// 					point[point_index].x = atk_rgblcd_get_lcd_width() - point_raw.x;
// 					point[point_index].y = point_raw.y;
// 					break;
// 				}
// 			}
// 		}

// 		return tp_cnt;
// 	} else {
// 		return 0;
// 	}
// }

// #endif
