#include "touch.h"

#include "delay.h"
#include "i2c.h"
#include "lcd.h"
#include "main.h"

uint8_t touch_iic_addr;

TouchFeedback_t touch_feedback = {0};

static void touch_hw_reset(uint8_t addr) {
	GPIO_InitTypeDef gpio_init_struct = {0};

	gpio_init_struct.Pin = TP_PEN_Pin;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Pull = GPIO_PULLUP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(TP_PEN_GPIO_Port, &gpio_init_struct);

	switch (addr) {
		case 0x28: {
			HAL_GPIO_WritePin(TP_CS_GPIO_Port, TP_CS_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TP_PEN_GPIO_Port, TP_PEN_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TP_PEN_GPIO_Port, TP_PEN_Pin, GPIO_PIN_SET);
			delay(1);
			HAL_GPIO_WritePin(TP_CS_GPIO_Port, TP_CS_Pin, GPIO_PIN_SET);
			delay(10);
			break;
		}
		case 0xBA: {
			HAL_GPIO_WritePin(TP_CS_GPIO_Port, TP_CS_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(TP_PEN_GPIO_Port, TP_PEN_Pin, GPIO_PIN_RESET);
			delay(1);
			HAL_GPIO_WritePin(TP_CS_GPIO_Port, TP_CS_Pin, GPIO_PIN_SET);
			delay(10);
			break;
		}
		default: {
			break;
		}
	}
	gpio_init_struct.Pin = TP_PEN_Pin;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;
	gpio_init_struct.Pull = GPIO_NOPULL;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(TP_PEN_GPIO_Port, &gpio_init_struct);

	touch_iic_addr = addr >> 1;
	delay(100);
}


static uint8_t touch_write_reg(uint16_t reg, uint8_t* buf, uint8_t len) {
	uint8_t buf_index;
	uint8_t ret;

	iic_start();
	iic_send_byte((touch_iic_addr << 1) | 0);
	iic_wait_ack();
	iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
	iic_wait_ack();
	iic_send_byte((uint8_t)reg & 0xFF);
	iic_wait_ack();

	for (buf_index = 0; buf_index < len; buf_index++) {
		iic_send_byte(buf[buf_index]);
		ret = iic_wait_ack();
		if (ret != 0) {
			break;
		}
	}

	iic_stop();

	if (ret != 0) {
		return 0;
	}

	return 1;
}

static void touch_read_reg(uint16_t reg, uint8_t* buf, uint8_t len) {
	uint8_t buf_index;

	iic_start();
	iic_send_byte((touch_iic_addr << 1) | 0);
	iic_wait_ack();
	iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
	iic_wait_ack();
	iic_send_byte((uint8_t)reg & 0xFF);
	iic_wait_ack();
	iic_start();
	iic_send_byte((touch_iic_addr << 1) | 1);
	iic_wait_ack();

	for (buf_index = 0; buf_index < len - 1; buf_index++) {
		buf[buf_index] = iic_read_byte(1);
	}

	buf[buf_index] = iic_read_byte(0);

	iic_stop();
}

static void touch_sw_reset(void) {
	uint8_t dat;

	dat = 0x02;
	touch_write_reg(REG_CTRL, &dat, 1);
	delay(10);

	dat = 0x00;
	touch_write_reg(REG_CTRL, &dat, 1);
}

static void touch_get_pid(char* pid) {
	touch_read_reg(REG_PID, (uint8_t*)pid, 4);
	pid[4] = '\0';
}

void touch_update(void) {
	// static uint8_t timer = 0;
	uint8_t i = 0;
	uint8_t _temp;
	uint8_t data_temp[6];
	static volatile uint32_t zero_counter = 0;

	// timer++;
	// if (timer < 10) {
	// 	return;
	// }
	// timer = 0;


	touch_read_reg(REG_TPINFO, &_temp, 1);

	// attempt at doing edge triggering
	// don't think this is supposed to work this way but idc rn
	// I don't actually know why this works
	touch_feedback.last_pressed_state = touch_feedback.pressed_state;
	zero_counter = (_temp & 0x80) ? 0 : zero_counter + 1;
	touch_feedback.pressed_state = zero_counter >= 20 ? 0 : 1;
	if (touch_feedback.last_pressed_state != touch_feedback.pressed_state) {
		touch_feedback.clicked = touch_feedback.pressed_state;
	} else {
		touch_feedback.clicked = 0;
	}

	touch_feedback.num = (_temp & 0x0f);

	// if touches are detected
	if (touch_feedback.pressed_state) {
		for (i = 0; i < touch_feedback.num; i++)
		// update point values
		{
			touch_read_reg(REG_TP1 + i * 8, data_temp, 6);
			touch_feedback.point[i].x = (uint16_t)(data_temp[1] << 8) | data_temp[0];
			touch_feedback.point[i].y = (uint16_t)(data_temp[3] << 8) | data_temp[2];
			touch_feedback.point[i].size = (uint16_t)(data_temp[5] << 8) | data_temp[4];
		}
		_temp = 0;
		touch_write_reg(REG_TPINFO, &_temp, 1);
	}
}


void touch_init(void) {
	char pid[5];
	soft_IIC_Init();
	touch_hw_reset(0x28);
	touch_get_pid(pid);
	touch_sw_reset();
}
