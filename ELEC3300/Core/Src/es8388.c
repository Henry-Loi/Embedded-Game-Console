/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in
 * which case, it is free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "es8388.h"

#include "I2C.h"
#include "delay.h"
#include "ff_gen_drv.h"

#include "stm32f4xx_hal.h"
// #include "stm32f4xx_hal_i2s.h"

#include <stdio.h>
#include <string.h>

I2S_HandleTypeDef hi2s2;
I2S_HandleTypeDef hi2S2ext;
DMA_HandleTypeDef hdma_i2s2Tx;
DMA_HandleTypeDef hdma_i2s2Rx;

volatile unsigned char AudioOutFlag;
volatile unsigned char TxCpltFlag;
volatile unsigned char TxHalfCpltFlag;
volatile unsigned char TxBufNum;

volatile unsigned char RxCpltFlag;
volatile unsigned char RxHalfCpltFlag;
volatile unsigned char RecordFlag = 0;

volatile unsigned char Audio_ErrorFlag;

__attribute__((aligned(4))) unsigned char audio_buffer[4096];
__attribute__((aligned(4))) unsigned short int audio_output_buffer[9216];

// static FILINFO file_information;
static FIL file_object;
__attribute__((aligned(4))) DWORD clmt[2400];

const unsigned int I2SFreq[8] = {8000, 11025, 16000, 22050, 32000, 44100, 48000, 96000};
/* These PLL parameters are valide when the f(VCO clock) = 1Mhz */
const unsigned int I2SPLLN[8] = {256, 429, 213, 429, 426, 271, 258, 344};
const unsigned int I2SPLLR[8] = {5, 4, 4, 4, 4, 6, 3, 1};

extern const unsigned char _acvolice1[102528UL + 1];

/**
 * @brief  ��ʱ��������ʱXms
 * @note
 * @retval None
 */
void ES8388_Delay_1ms(unsigned int ms) { delay(ms); }

/**
* @brief  es8288д�Ĵ���
* @param reg_add:�Ĵ�����ַ
								 data:Ҫд��Ĵ�����ֵ
* @note
* @retval 0:�ɹ� ����: �������
*/
static unsigned char es8388_write_reg(unsigned char reg_add, unsigned char data) {
	I2C_Start();

	I2C_SendByte((ES8388_ADDR | 0)); // ����������ַ+д����
	if (Check_ACK() != 1) {
		I2C_Stop();
		// printf("es8388 write reg err!!!\r\n");
		return 1;
	}

	I2C_SendByte(reg_add);
	if (Check_ACK() != 1) {
		I2C_Stop();
		// printf("es8388 write reg err!!!\r\n");
		return 1;
	}

	I2C_SendByte(data);
	if (Check_ACK() != 1) {
		I2C_Stop();
		// printf("es8388 write reg err!!!\r\n");
		return 1;
	}

	I2C_Stop();
	return 0;
}

/**
* @brief  es8388���Ĵ���
* @param reg_add:�Ĵ�����ַ
								 p_data:Ҫ��ȡ�Ĵ�����ֵ
* @note
* @retval 0:�ɹ� ����: �������
*/
static unsigned char es8388_read_reg(unsigned char reg_add, unsigned char* p_data) {
	I2C_Start();

	I2C_SendByte((ES8388_ADDR | 0)); // ����������ַ+д����
	if (Check_ACK() != 1) {
		I2C_Stop();
		// printf("es8388 read reg err!!!\r\n");
		return 1;
	}

	I2C_SendByte(reg_add);
	if (Check_ACK() != 1) {
		I2C_Stop();
		// printf("es8388 read reg err!!!\r\n");
		return 1;
	}

	I2C_Start();

	I2C_SendByte((ES8388_ADDR | 1)); // ����������ַ+������
	if (Check_ACK() != 1) {
		I2C_Stop();
		// printf("es8388 read reg err!!!\r\n");
		return 1;
	}

	*p_data = I2C_ReciveByte();

	I2C_SendNACK();

	I2C_Stop();

	return 0;
}

/**
 * @brief  ��ʾes8388ȫ���Ĵ�����ֵ
 * @param none
 * @note
 * @retval none
 */
void es8388_read_all() {
	unsigned char reg = 0;
	int i;

	// printf("\r\n");
	for (i = 0; i < 50; i++) {
		reg = 0;
		if (es8388_read_reg(i, &reg) == 0) {
			// printf("reg: %x,val: %x\r\n", i, reg);
		}
	}
	// printf("\r\n");
}

/**
 * @brief Configure ES8388 ADC and DAC volume. Basicly you can consider this as
 * ADC and DAC gain
 *
 * @param mode:             set ADC or DAC or all
 * @param volume:           -96 ~ 0              for example
 * Es8388SetAdcDacVolume(ES_MODULE_ADC, 30, 6); means set ADC volume -30.5db
 * @param dot:              whether include 0.5. for example
 * Es8388SetAdcDacVolume(ES_MODULE_ADC, 30, 4); means set ADC volume -30db
 *
 * @return
 *     - (1) Parameter error
 *     - (0)   Success
 */
unsigned char es8388_set_adc_dac_volume(int mode, signed int volume, unsigned int dot) {
	int res = 0;

	if (volume < -96 || volume > 0) {
		// printf("Warning: volume < -96! or > 0!\r\n");
		if (volume < -96)
			volume = -96;
		else
			volume = 0;
	}

	dot = (dot >= 5 ? 1 : 0);
	volume = (-volume << 1) + dot;
	if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
		res |= es8388_write_reg(ES8388_ADCCONTROL8, volume);
		res |= es8388_write_reg(ES8388_ADCCONTROL9, volume); // ADC Right Volume=0db
	}
	if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
		res |= es8388_write_reg(ES8388_DACCONTROL5, volume);
		res |= es8388_write_reg(ES8388_DACCONTROL4, volume);
	}
	return res;
}

/**
 * @brief  Start ES8388 codec chip
 *
 * @param mode:  set ADC or DAC or both
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_start(es_module_t mode) {
	unsigned char res = 0;
	unsigned char prev_data = 0, data = 0;

	es8388_read_reg(ES8388_DACCONTROL21, &prev_data);
	if (mode == ES_MODULE_LINE) {
		res |= es8388_write_reg(ES8388_DACCONTROL16,
								0x09); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2 by pass enable
		res |= es8388_write_reg(ES8388_DACCONTROL17,
								0x50); // left DAC to left mixer enable  and  LIN signal to left mixer
									   // enable 0db  : bupass enable
		res |= es8388_write_reg(ES8388_DACCONTROL20,
								0x50);						// right DAC to right mixer enable  and  LIN signal to right
															// mixer enable 0db : bupass enable
		res |= es8388_write_reg(ES8388_DACCONTROL21, 0xC0); // enable adc
	} else {
		res |= es8388_write_reg(ES8388_DACCONTROL21, 0x80); // enable dac
	}
	es8388_read_reg(ES8388_DACCONTROL21, &data);
	if (prev_data != data) {
		res |= es8388_write_reg(ES8388_CHIPPOWER, 0xF0); // start state machine
		// res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL1, 0x16);
		// res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL2, 0x50);
		res |= es8388_write_reg(ES8388_CHIPPOWER, 0x00); // start state machine
	}
	if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC || mode == ES_MODULE_LINE) {
		res |= es8388_write_reg(ES8388_ADCPOWER, 0x00); // power up adc and line in
	}
	if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC || mode == ES_MODULE_LINE) {
		//      res |= es8388_write_reg(ES8388_DACPOWER, 0x3c);   //power up dac and
		//      line out
		res |= es8388_set_voice_mute(0);
		// printf("es8388_start default is mode:%d\r\n", mode);
	}

	return res;
}

/**
 * @brief  Stop ES8388 codec chip
 *
 * @param mode:  set ADC or DAC or both
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_stop(es_module_t mode) {
	unsigned char res = 0;

	if (mode == ES_MODULE_LINE) {
		res |= es8388_write_reg(ES8388_DACCONTROL21, 0x80); // enable dac
		res |= es8388_write_reg(ES8388_DACCONTROL16,
								0x00); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
		res |= es8388_write_reg(ES8388_DACCONTROL17,
								0x90); // only left DAC to left mixer enable 0db
		res |= es8388_write_reg(ES8388_DACCONTROL20,
								0x90); // only right DAC to right mixer enable 0db
		return res;
	}

	if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
		res |= es8388_write_reg(ES8388_DACPOWER, 0x00);
		res |= es8388_set_voice_mute(1); // res |= Es8388SetAdcDacVolume(ES_MODULE_DAC, -96, 5);      // 0db
										 // res |= es_write_reg(ES8388_ADDR, ES8388_DACPOWER, 0xC0);  //power down
										 // dac and line out
	}

	if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
		// res |= Es8388SetAdcDacVolume(ES_MODULE_ADC, -96, 5);      // 0db
		res |= es8388_write_reg(ES8388_ADCPOWER, 0xFF); // power down adc and line
														// in
	}
	if (mode == ES_MODULE_ADC_DAC) {
		res |= es8388_write_reg(ES8388_DACCONTROL21, 0x9C); // disable mclk
		//        res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL1, 0x00);
		//        res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL2, 0x58);
		//        res |= es_write_reg(ES8388_ADDR, ES8388_CHIPPOWER, 0xF3);  //stop
		//        state machine
	}

	return res;
}

/**
 * @brief Config I2s clock in MSATER mode
 *
 * @param cfg.sclkDiv:      generate SCLK by dividing MCLK in MSATER mode
 * @param cfg.lclkDiv:      generate LCLK by dividing MCLK in MSATER mode
 *
 * @return
 *     - (1)  Error
 *     - (0)  Success
 */
unsigned char es8388_i2s_config_clock(es_i2s_clock_t cfg) {
	unsigned char res = 0;

	res |= es8388_write_reg(ES8388_MASTERMODE, cfg.sclk_div);
	res |= es8388_write_reg(ES8388_ADCCONTROL5,
							cfg.lclk_div); // ADCFsMode,singel SPEED,RATIO=256
	res |= es8388_write_reg(ES8388_DACCONTROL2,
							cfg.lclk_div); // ADCFsMode,singel SPEED,RATIO=256
	return res;
}

/**
 * @brief Deinitialize ES8388 codec chip
 *
 * @return
 *     - (1)  Error
 *     - (0)  Success
 */
unsigned char es8388_deinit(void) {
	int res = 0;
	res = es8388_write_reg(ES8388_CHIPPOWER, 0xFF); // reset and stop es8388
	return res;
}

/**
 * @brief Initialize ES8388 codec chip
 *
 * @param dac_out: �������ͨ��ѡ��
 * @param adc_in: ��˷�����ͨ��ѡ��
 * @param mode: es8388��I2S����ģʽѡ��
 *
 * @return
 *     - (1)  Error
 *     - (0)  Success
 */
unsigned char es8388_init(audio_hal_dac_output_t dac_out, audio_hal_adc_input_t adc_in, audio_hal_iface_mode_t mode) {
	int res = 0;
	int tmp;

	res |= es8388_write_reg(ES8388_DACCONTROL3,
							0x04); // 0x04 mute/0x00 unmute&ramp;DAC unmute and
								   // disabled digital volume control soft ramp
	/* Chip Control and Power Management */
	res |= es8388_write_reg(ES8388_CONTROL2, 0x50);
	res |= es8388_write_reg(ES8388_CHIPPOWER, 0x00);  // normal all and power up
													  // all
	res |= es8388_write_reg(ES8388_MASTERMODE, mode); // CODEC IN I2S SLAVE MODE

	/* dac */
	if (AUDIO_HAL_DAC_OUTPUT_NONE != dac_out) {
		res |= es8388_write_reg(ES8388_DACPOWER,
								0xC0); // disable DAC and disable Lout/Rout/1/2
		res |= es8388_write_reg(ES8388_CONTROL1,
								0x12); // Enfr=0,Play&Record Mode,(0x17-both of mic&paly)
		//    res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL2, 0);
		//    //LPVrefBuf=0,Pdn_ana=0
		res |= es8388_write_reg(ES8388_DACCONTROL1,
								0x18); // 1a 0x18:16bit iis , 0x00:24
		res |= es8388_write_reg(ES8388_DACCONTROL2,
								0x02); // DACFsMode,SINGLE SPEED; DACFsRatio,256
		res |= es8388_write_reg(ES8388_DACCONTROL16,
								0x00); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
		res |= es8388_write_reg(ES8388_DACCONTROL17,
								0x90); // only left DAC to left mixer enable 0db
		res |= es8388_write_reg(ES8388_DACCONTROL20,
								0x90); // only right DAC to right mixer enable 0db
		res |= es8388_write_reg(ES8388_DACCONTROL21,
								0x80);						   // set internal ADC and DAC use the same LRCK
															   // clock, ADC LRCK as internal LRCK
		res |= es8388_write_reg(ES8388_DACCONTROL23, 0x00);	   // vroi=0
		res |= es8388_set_adc_dac_volume(ES_MODULE_DAC, 0, 0); // 0db
		res |= es8388_write_reg(ES8388_DACCONTROL24, 0);
		res |= es8388_write_reg(ES8388_DACCONTROL25, 0);
		res |= es8388_write_reg(ES8388_DACCONTROL26, 0);
		res |= es8388_write_reg(ES8388_DACCONTROL27, 0);

		tmp = 0;
		if (AUDIO_HAL_DAC_OUTPUT_LINE2 == dac_out) {
			tmp = DAC_OUTPUT_LOUT2 | DAC_OUTPUT_ROUT2;
		} else if (AUDIO_HAL_DAC_OUTPUT_LINE1 == dac_out) {
			tmp = DAC_OUTPUT_LOUT1 | DAC_OUTPUT_ROUT1;
		} else {
			tmp = DAC_OUTPUT_LOUT1 | DAC_OUTPUT_LOUT2 | DAC_OUTPUT_ROUT1 | DAC_OUTPUT_ROUT2;
		}

		res |= es8388_write_reg(ES8388_DACPOWER,
								tmp); // 0x3c Enable DAC and Enable Lout/Rout/1/2
	}

	/* adc */
	if (AUDIO_HAL_ADC_INPUT_NONE != adc_in) {
		res |= es8388_write_reg(ES8388_ADCPOWER, 0xFF);
		res |= es8388_write_reg(ES8388_ADCCONTROL1,
								0xbb); // MIC Left and Right channel PGA gain

		tmp = 0;
		if (AUDIO_HAL_ADC_INPUT_LINE1 == adc_in) {
			tmp = ADC_INPUT_LINPUT1_RINPUT1;
		} else if (AUDIO_HAL_ADC_INPUT_LINE2 == adc_in) {
			tmp = ADC_INPUT_LINPUT2_RINPUT2;
		} else {
			tmp = ADC_INPUT_DIFFERENCE;
		}

		res |= es8388_write_reg(ES8388_ADCCONTROL2,
								tmp); // 0x00 LINSEL & RINSEL, LIN1/RIN1 as ADC Input;
									  // DSSEL,use one DS Reg11; DSR, LINPUT1-RINPUT1
		res |= es8388_write_reg(ES8388_ADCCONTROL3, 0x02);
		res |= es8388_write_reg(ES8388_ADCCONTROL4,
								0x0d); // Left/Right data, Left/Right justified
									   // mode, Bits length, I2S format
		res |= es8388_write_reg(ES8388_ADCCONTROL5,
								0x02); // ADCFsMode,singel SPEED,RATIO=256

		// ALC for Microphone
		res |= es8388_set_adc_dac_volume(ES_MODULE_ADC, 0, 0); // 0db
		res |= es8388_write_reg(ES8388_ADCPOWER,
								0x09); // Power on ADC, Enable LIN&RIN, Power off
									   // MICBIAS, set int1lp to low power mode
	}

	//    /* enable es8388 PA */
	//    es8388_pa_power(true);
	// printf("init,out:%02x, in:%02x\r\n", dac_out, adc_in);
	return res;
}

/**
 * @brief Configure ES8388 I2S format
 *
 * @param mode:  set ADC or DAC or all
 * @param fmt:   i2sЭ��
 *
 * @return
 *     - (1) Error
 *     - (0) Success
 */
unsigned char es8388_config_fmt(es_module_t mode, es_i2s_fmt_t fmt) {
	unsigned char res = 0;
	uint8_t reg = 0;

	if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
		res = es8388_read_reg(ES8388_ADCCONTROL4, &reg);
		reg = reg & 0xfc;
		res |= es8388_write_reg(ES8388_ADCCONTROL4, reg | fmt);
	}
	if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
		res = es8388_read_reg(ES8388_DACCONTROL1, &reg);
		reg = reg & 0xf9;
		res |= es8388_write_reg(ES8388_DACCONTROL1, reg | (fmt << 1));
	}
	return res;
}

/**
 * @brief ����ES8388�����������
 *
 * @param dac_out: �������ͨ��ѡ��
 * @param volume: 0 ~ 100
 *
 * @return
 *     - (1)  Error
 *     - (0)   Success
 */
unsigned char es8388_set_voice_volume(audio_hal_dac_output_t dac_out, int volume) {
	unsigned char res = 1;

	if (volume < 0)
		volume = 0;
	else if (volume > 100)
		volume = 100;
	volume /= 3;

	if (dac_out == AUDIO_HAL_DAC_OUTPUT_LINE1) {
		res = es8388_write_reg(ES8388_DACCONTROL24, volume);
		res |= es8388_write_reg(ES8388_DACCONTROL25, volume);
	}

	if (dac_out == AUDIO_HAL_DAC_OUTPUT_LINE2) {
		res = es8388_write_reg(ES8388_DACCONTROL26, volume);
		res |= es8388_write_reg(ES8388_DACCONTROL27, volume);
	}

	if (dac_out == AUDIO_HAL_DAC_OUTPUT_ALL) {
		res = es8388_write_reg(ES8388_DACCONTROL24, volume);
		res |= es8388_write_reg(ES8388_DACCONTROL25, volume);
		res |= es8388_write_reg(ES8388_DACCONTROL26, volume);
		res |= es8388_write_reg(ES8388_DACCONTROL27, volume);
	}

	return res;
}

/**
 * @brief Get voice volume
 *
 * @param dac_out: �������ͨ��ѡ��
 * @param[out] *volume:  voice volume (0~100)
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_get_voice_volume(audio_hal_dac_output_t dac_out, int* volume) {
	unsigned char res = 1;
	unsigned char reg = 0;

	if (dac_out == AUDIO_HAL_DAC_OUTPUT_LINE1 || dac_out == AUDIO_HAL_DAC_OUTPUT_ALL) {
		res = es8388_read_reg(ES8388_DACCONTROL24, &reg);
	}

	if (dac_out == AUDIO_HAL_DAC_OUTPUT_LINE2) {
		res = es8388_read_reg(ES8388_DACCONTROL26, &reg);
	}

	if (res == 1) {
		*volume = 0;
	} else {
		*volume = reg;
		*volume *= 3;
		if (*volume == 99)
			*volume = 100;
	}
	return res;
}

/**
 * @brief Configure ES8388 data sample bits
 *
 * @param mode:        set ADC or DAC or all
 * @param bits_length: ����λ
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_set_bits_per_sample(es_module_t mode, es_bits_length_t bits_length) {
	unsigned char res = 0;
	unsigned char reg = 0;
	int bits = (int)bits_length;

	if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
		res = es8388_read_reg(ES8388_ADCCONTROL4, &reg);
		reg = reg & 0xe3;
		res |= es8388_write_reg(ES8388_ADCCONTROL4, reg | (bits << 2));
	}
	if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
		res = es8388_read_reg(ES8388_DACCONTROL1, &reg);
		reg = reg & 0xc7;
		res |= es8388_write_reg(ES8388_DACCONTROL1, reg | (bits << 3));
	}
	return res;
}

/**
 * @brief Configure ES8388 DAC mute or not. Basically you can use this function
 * to mute the output or unmute
 *
 * @param enable: enable or disable
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_set_voice_mute(unsigned char enable) {
	unsigned char res = 0;
	unsigned char reg = 0;

	res = es8388_read_reg(ES8388_DACCONTROL3, &reg);
	reg = reg & 0xFB;
	res |= es8388_write_reg(ES8388_DACCONTROL3, reg | (((int)enable) << 2));
	return res;
}

/**
 * @brief Get ES8388 DAC mute status
 *
 *  @return
 *     - 1 Parameter error
 *     - 0 Success
 */
unsigned char es8388_get_voice_mute(void) {
	unsigned char res = 0;
	unsigned char reg = 0;
	res = es8388_read_reg(ES8388_DACCONTROL3, &reg);
	if (res == 0) {
		reg = (reg & 0x04) >> 2;
	}
	return res == 0 ? reg : res;
}

/**
 * @brief ������˷�����
 *
 * @param gain: see es_mic_gain_t
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_set_mic_gain(es_mic_gain_t gain) {
	unsigned char res, gain_n;

	gain_n = (int)gain / 3;
	res = es8388_write_reg(ES8388_ADCCONTROL1, gain_n); // MIC PGA
	return res;
}

/**
 * @brief Configure ES8388 I2S interface
 *
 * @param mode:  set ADC or DAC or all
 * @param fmt: i2sЭ��
 * @param bits: ����λ
 *
 * @return
 *     - 1 Parameter error
 *     - 0 Success
 */
unsigned char es8388_config_i2s(es_module_t mode, es_i2s_fmt_t fmt, audio_hal_iface_bits_t bits) {
	unsigned char res = 0;
	es_bits_length_t tmp;

	res |= es8388_config_fmt(mode, fmt);
	if (bits == AUDIO_HAL_BIT_LENGTH_16BITS) {
		tmp = BIT_LENGTH_16BITS;
	} else if (bits == AUDIO_HAL_BIT_LENGTH_24BITS) {
		tmp = BIT_LENGTH_24BITS;
	} else {
		tmp = BIT_LENGTH_32BITS;
	}
	res |= es8388_set_bits_per_sample(mode, tmp);
	return res;
}

/**
 * @brief  ����MCU��es8388�Ľӿ�.
 * @note
 * @retval None
 */
void ES8388_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	HAL_GPIO_DeInit(GPIOE, (GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_6));
	ES8388_Delay_1ms(10);

	GPIO_InitStruct.Pin = GPIO_PIN_2; // PC6--I2S2_MCK
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5; // PB13--I2S2_SCLK
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_4; // PB12--I2S2_LRCLK
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3; // PC2--I2S2_SDOUT
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Alternate = GPIO_AF6_I2S2ext;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6; // PC3--I2S2_SDIN
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/**
 * @brief  I2S2�������ͳ�ʼ��
 * @param AudioFreq: ������
 * @note
 * @retval
 */
void MCU_I2S2_TX_Init(unsigned int AudioFreq) {
	RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;
	unsigned char index = 0, freqindex = 0xFF;

	/* Disable I2S peripheral */
	__HAL_I2S_DISABLE(&hi2s2);

	hi2s2.Instance = SPI2;
	HAL_I2S_DeInit(&hi2s2);

	__HAL_RCC_SPI2_CLK_ENABLE();

	/** Sw reset of I2S2 IP */
	__HAL_RCC_SPI2_FORCE_RESET();
	ES8388_Delay_1ms(10);
	__HAL_RCC_SPI2_RELEASE_RESET();
	ES8388_Delay_1ms(10);

	/* Clock Config */
	for (index = 0; index < 8; index++) {
		if (I2SFreq[index] == AudioFreq) {
			freqindex = index;
		}
	}

	HAL_RCCEx_GetPeriphCLKConfig(&RCC_ExCLKInitStruct);
	if (freqindex != 0xFF) {
		/* I2S clock config
		PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) ?(PLLI2SN/PLLM)
		I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
		RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = I2SPLLN[freqindex];
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SR = I2SPLLR[freqindex];
		HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct);
	} else /* Default PLL I2S configuration */
	{
		/* I2S clock config
		PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) ?(PLLI2SN/PLLM)
		I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
		RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = 258;
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SR = 3;
		HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct);
	}

	hi2s2.Instance = SPI2;
	hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
	hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B;

#if (USE_12MHZ_XTAL == 0)
	hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
#else
	hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
#endif

	hi2s2.Init.AudioFreq = AudioFreq;
	hi2s2.Init.CPOL = I2S_CPOL_LOW;
	hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
	HAL_I2S_Init(&hi2s2);

	/* Enable the DMA clock */
	__HAL_RCC_DMA1_CLK_ENABLE();

	hdma_i2s2Tx.Instance = DMA1_Stream4;
	/* Deinitialize the Stream for new transfer */
	HAL_DMA_DeInit(&hdma_i2s2Tx);

	/* Configure the hdma_i2sTx handle parameters */
	hdma_i2s2Tx.Init.Channel = DMA_CHANNEL_0;
	hdma_i2s2Tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_i2s2Tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_i2s2Tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_i2s2Tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_i2s2Tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_i2s2Tx.Init.Mode = DMA_CIRCULAR;
	hdma_i2s2Tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	hdma_i2s2Tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	hdma_i2s2Tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_i2s2Tx.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_i2s2Tx.Init.PeriphBurst = DMA_PBURST_SINGLE;

	/* Associate the DMA handle */
	__HAL_LINKDMA(&hi2s2, hdmatx, hdma_i2s2Tx);

	/* Configure the DMA Stream */
	HAL_DMA_Init(&hdma_i2s2Tx);

	/* I2S DMA IRQ Channel configuration */
	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
}

/**
 * @brief  I2S2�������ճ�ʼ��
 * @param AudioFreq: ������
 * @note
 * @retval
 */
void MCU_I2S2_RX_Init(unsigned int AudioFreq) {
	RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;
	unsigned char index = 0, freqindex = 0xFF;

	/* Disable I2S peripheral */
	__HAL_I2S_DISABLE(&hi2S2ext);

	hi2S2ext.Instance = I2S2ext;
	HAL_I2S_DeInit(&hi2S2ext);

	__HAL_RCC_SPI2_CLK_ENABLE();
	//
	//	/** Sw reset of I2S2 IP */
	//	__HAL_RCC_SPI2_FORCE_RESET();
	//	WM8978_Delay_1ms( 10 );
	//  __HAL_RCC_SPI2_RELEASE_RESET();
	//	WM8978_Delay_1ms( 10 );

	/* Clock Config */
	for (index = 0; index < 8; index++) {
		if (I2SFreq[index] == AudioFreq) {
			freqindex = index;
		}
	}

	HAL_RCCEx_GetPeriphCLKConfig(&RCC_ExCLKInitStruct);
	if (freqindex != 0xFF) {
		/* I2S clock config
		PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) ?(PLLI2SN/PLLM)
		I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
		RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = I2SPLLN[freqindex];
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SR = I2SPLLR[freqindex];
		HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct);
	} else /* Default PLL I2S configuration */
	{
		/* I2S clock config
		PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) ?(PLLI2SN/PLLM)
		I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
		RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = 258;
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SR = 3;
		HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct);
	}

	hi2S2ext.Instance = I2S2ext;
	hi2S2ext.Init.Mode = I2S_MODE_SLAVE_RX;
	hi2S2ext.Init.Standard = I2S_STANDARD_PHILIPS;
	hi2S2ext.Init.DataFormat = I2S_DATAFORMAT_16B;
	hi2S2ext.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;

#if (USE_12MHZ_XTAL == 0) // ʹ���ڲ�������MCLK��¼��ʱ������������
	hi2S2ext.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
#else
	hi2S2ext.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
#endif

	hi2S2ext.Init.AudioFreq = AudioFreq;
	hi2S2ext.Init.CPOL = I2S_CPOL_LOW;
	hi2S2ext.Init.ClockSource = I2S_CLOCK_PLL;
	HAL_I2S_Init(&hi2S2ext);

	/* Enable the DMA clock */
	__HAL_RCC_DMA1_CLK_ENABLE();

	hdma_i2s2Rx.Instance = DMA1_Stream3;
	/* Deinitialize the Stream for new transfer */
	HAL_DMA_DeInit(&hdma_i2s2Rx);

	/* Configure the hdma_i2sTx handle parameters */
	hdma_i2s2Rx.Init.Channel = DMA_CHANNEL_3;
	hdma_i2s2Rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_i2s2Rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_i2s2Rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_i2s2Rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_i2s2Rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_i2s2Rx.Init.Mode = DMA_CIRCULAR;
	hdma_i2s2Rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	hdma_i2s2Rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_i2s2Rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_i2s2Rx.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_i2s2Rx.Init.PeriphBurst = DMA_PBURST_SINGLE;

	/* Associate the DMA handle */
	__HAL_LINKDMA(&hi2S2ext, hdmarx, hdma_i2s2Rx);

	/* Configure the DMA Stream */
	HAL_DMA_Init(&hdma_i2s2Rx);

	/* I2S DMA IRQ Channel configuration */
	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
}

/**
 * @brief  Starts playing audio stream from a data buffer for a determined size.
 * @param  pBuffer: Pointer to the buffer
 * @param  Size: Number of audio data BYTES.
 * @retval AUDIO_OK if correct communication, else wrong communication
 */
unsigned char BSP_AUDIO_OUT_Play(unsigned short int* pBuffer, unsigned int Size) {
	HAL_I2S_Transmit_DMA(&hi2s2, pBuffer, (Size / 2));
	return 0;
}

/**
 * @brief   Pauses the audio file stream.
 *          In case of using DMA, the DMA Pause feature is used.
 * WARNING: When calling BSP_AUDIO_OUT_Pause() function for pause, only
 *          BSP_AUDIO_OUT_Resume() function should be called for resume (use of
 * BSP_AUDIO_OUT_Play() function for resume could lead to unexpected behavior).
 * @retval  AUDIO_OK if correct communication, else wrong communication
 */
unsigned char BSP_AUDIO_OUT_Pause(void) {
	HAL_I2S_DMAPause(&hi2s2);
	return 0;
}

/**
 * @brief   Resumes the audio file stream.
 * WARNING: When calling BSP_AUDIO_OUT_Pause() function for pause, only
 *          BSP_AUDIO_OUT_Resume() function should be called for resume (use of
 * BSP_AUDIO_OUT_Play() function for resume could lead to unexpected behavior).
 * @retval  AUDIO_OK if correct communication, else wrong communication
 */
unsigned char BSP_AUDIO_OUT_Resume(void) {
	HAL_I2S_DMAResume(&hi2s2);
	return 0;
}

/**
 * @brief  Stops audio playing and Power down the Audio Codec.
 * @param  Option: could be one of the following parameters
 *           - CODEC_PDWN_SW: for software power off (by writing registers).
 *                            Then no need to reconfigure the Codec after power
 * on.
 *           - CODEC_PDWN_HW: completely shut down the codec (physically).
 *                            Then need to reconfigure the Codec after power on.
 * @retval AUDIO_OK if correct communication, else wrong communication
 */
unsigned char BSP_AUDIO_OUT_Stop(unsigned int Option) {
	HAL_I2S_DMAStop(&hi2s2);
	es8388_stop(ES_MODULE_DAC);
	return 0;
}

/**
 * @brief Tx Transfer completed callbacks
 * @param hi2s: I2S handle
 */
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef* hi2s) {
	/* Manage the remaining file size and new address offset: This function
	   should be coded by user (its prototype is already declared in
	   stm324xg_eval_audio.h) */

	if (RecordFlag == 0) {
		TxCpltFlag = 1;
		AudioOutFlag = 0;
		TxBufNum = 0;

		if (TxHalfCpltFlag == 1) {
			BSP_AUDIO_OUT_Pause();
			TxBufNum = 1;
		}
	}
}

/**
 * @brief Tx Transfer Half completed callbacks
 * @param hi2s: I2S handle
 */
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef* hi2s) {
	/* Manage the remaining file size and new address offset: This function
	   should be coded by user (its prototype is already declared in
	   stm324xg_eval_audio.h) */

	if (RecordFlag == 0) {
		TxHalfCpltFlag = 1;
		AudioOutFlag = 0;
		TxBufNum = 1;

		if (TxCpltFlag == 1) {
			BSP_AUDIO_OUT_Pause();
			TxBufNum = 0;
		}
	}
}

/**
 * @brief  I2S error callbacks.
 * @param  hi2s: I2S handle
 */
void HAL_I2S_ErrorCallback(I2S_HandleTypeDef* hi2s) { Audio_ErrorFlag = 1; }

/**
 * @brief  ������������ת��Ϊ������
 * @param  buffer:����������
 * @param  num:����������
 * @retval none
 */
void Audio_Convert_Mono(unsigned short int* buffer, unsigned int num) {
	int i;
	for (i = num - 1; i >= 0; i--) {
		buffer[i * 2] = buffer[i];
		buffer[i * 2 + 1] = buffer[i];
	}
}

/**
 * @brief  Starts audio recording.
 * @param  pbuf: Main buffer pointer for the recorded data storing
 * @param  size: size of the recorded buffer in number of elements (typically
 * number of half-words) Be careful that it is not the same unit than
 * BSP_AUDIO_OUT_Play function
 * @retval AUDIO_OK if correct communication, else wrong communication
 */
unsigned char BSP_AUDIO_IN_Record(unsigned short int* pbuf, unsigned int size) {
	HAL_I2S_Receive_DMA(&hi2S2ext, pbuf, size);
	return 0;
}

/**
 * @brief  Stops audio recording.
 * @param  Option: could be one of the following parameters
 *           - CODEC_PDWN_SW: for software power off (by writing registers).
 *                            Then no need to reconfigure the Codec after power
 * on.
 *           - CODEC_PDWN_HW: completely shut down the codec (physically).
 *                            Then need to reconfigure the Codec after power on.
 * @retval AUDIO_OK if correct communication, else wrong communication
 */
unsigned char BSP_AUDIO_IN_Stop(unsigned char Option) {
	HAL_I2S_DMAStop(&hi2S2ext);
	es8388_stop(ES_MODULE_ADC);
	return 0;
}

/**
 * @brief  Pauses the audio file stream.
 * @retval AUDIO_OK if correct communication, else wrong communication
 */
unsigned char BSP_AUDIO_IN_Pause(void) {
	/* Call the Media layer pause function */
	HAL_I2S_DMAPause(&hi2S2ext);
	/* Return AUDIO_OK when all operations are correctly done */
	return 0;
}

/**
 * @brief  Resumes the audio file stream.
 * @retval AUDIO_OK if correct communication, else wrong communication
 */
unsigned char BSP_AUDIO_IN_Resume(void) {
	/* Call the Media layer pause/resume function */
	HAL_I2S_DMAResume(&hi2S2ext);
	/* Return AUDIO_OK when all operations are correctly done */
	return 0;
}

/**
 * @brief Rx Transfer half completed callbacks
 * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
 *         the configuration information for I2S module
 * @retval None
 */
// void I2Sxext_DMAConvHalfCplt(DMA_HandleTypeDef *hdma)
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef* hi2s) { RxHalfCpltFlag = 1; }

/**
 * @brief Rx Transfer completed callbacks
 * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
 *         the configuration information for I2S module
 * @retval None
 */
// void I2Sxext_DMAConvCplt(DMA_HandleTypeDef *hdma)
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef* hi2s) { RxCpltFlag = 1; }

/**
* @brief ����PCM��ʽ��Ƶ���ݣ�������16K����������
* @param  dat: ��Ƶ����
		  size����Ƶ���ݴ�С
* @note
* @retval  1--�ɹ�  0--ʧ��
*/
unsigned char play_pcm(unsigned char* dat, unsigned int size) {
	unsigned int FrameNum, i, num;
	unsigned int dat_addr;
	unsigned char flag = 0;
	unsigned short int* p;

	num = size / 2;

	AudioOutFlag = 0;
	TxHalfCpltFlag = 0;
	TxCpltFlag = 0;
	TxBufNum = 0;
	Audio_ErrorFlag = 0;
	if (dat != NULL)
		dat_addr = (unsigned int)dat;

	while (num != 0) {
		if (flag == 0) {
			i = 4096;
		} else {
			if (num > 2048)
				i = 2048;
			else
				i = num;
		}

		if (flag == 0) {
			if (dat != NULL) {
				memcpy((unsigned char*)audio_output_buffer, (unsigned char*)dat_addr, i * 2);
				dat_addr = dat_addr + i * 2;
			}

			// ����������ת��Ϊ������
			Audio_Convert_Mono(audio_output_buffer, i * 2 / 2);

			p = audio_output_buffer;
			FrameNum += i;
			num = num - i;

			BSP_AUDIO_OUT_Play(p, (i * 2 * 2));

			flag = 1;
			AudioOutFlag = 1;
		} else {
			if (TxHalfCpltFlag == 1 && TxCpltFlag == 1) {
				if (TxBufNum == 0) {
					if (num >= 4096)
						i = 4096;
					else
						i = num;

					if (dat != NULL) {
						memcpy((unsigned char*)audio_output_buffer, (unsigned char*)dat_addr, i * 2);
						dat_addr = dat_addr + i * 2;
					}

					// ����������ת��Ϊ������
					Audio_Convert_Mono(audio_output_buffer, i * 2 / 2);

					FrameNum += i;
					num = num - i;
				} else if (TxBufNum == 1) {
					if (num >= 2048)
						i = 2048;
					else
						i = num;

					if (dat != NULL) {
						memcpy((unsigned char*)&audio_output_buffer[4096], (unsigned char*)dat_addr, i * 2);
						dat_addr = dat_addr + i * 2;
					}

					// ����������ת��Ϊ������
					Audio_Convert_Mono(&audio_output_buffer[4096], i * 2 / 2);

					FrameNum += i;
					num = num - i;

					if (num != 0) {
						if (num >= 2048)
							i = 2048;
						else
							i = num;

						if (dat != NULL) {
							memcpy((unsigned char*)audio_output_buffer, (unsigned char*)dat_addr, i * 2);
							dat_addr = dat_addr + i * 2;
						}

						// ����������ת��Ϊ������
						Audio_Convert_Mono(audio_output_buffer, i * 2 / 2);

						FrameNum += i;
						num = num - i;
					}
				}

				AudioOutFlag = 1;
				TxHalfCpltFlag = 0;
				TxCpltFlag = 0;

				BSP_AUDIO_OUT_Resume();
			} else if (TxHalfCpltFlag == 1 && TxCpltFlag == 0) {
				if (dat != NULL) {
					memcpy((unsigned char*)audio_output_buffer, (unsigned char*)dat_addr, i * 2);
					dat_addr = dat_addr + i * 2;
				}

				// ����������ת��Ϊ������
				Audio_Convert_Mono(audio_output_buffer, i * 2 / 2);

				FrameNum += i;
				num = num - i;
				AudioOutFlag = 1;
				TxHalfCpltFlag = 0;

				if (num == 0 && TxCpltFlag == 1) {
					TxCpltFlag = 0;

					BSP_AUDIO_OUT_Resume();
				}

				if (TxCpltFlag == 1) {
					if (num > 2048)
						i = 2048;
					else
						i = num;

					if (dat != NULL) {
						memcpy((unsigned char*)&audio_output_buffer[4096], (unsigned char*)dat_addr, i * 2);
						dat_addr = dat_addr + i * 2;
					}

					// ����������ת��Ϊ������
					Audio_Convert_Mono(&audio_output_buffer[4096], i * 2 / 2);

					FrameNum += i;
					num = num - i;

					TxCpltFlag = 0;

					BSP_AUDIO_OUT_Resume();
				}
			} else if (TxCpltFlag == 1 && TxHalfCpltFlag == 0) {
				if (dat != NULL) {
					memcpy((unsigned char*)&audio_output_buffer[4096], (unsigned char*)dat_addr, i * 2);
					dat_addr = dat_addr + i * 2;
				}

				// ����������ת��Ϊ������
				Audio_Convert_Mono(&audio_output_buffer[4096], i * 2 / 2);

				FrameNum += i;
				num = num - i;
				AudioOutFlag = 1;
				TxCpltFlag = 0;

				if (num == 0 && TxHalfCpltFlag == 1) {
					TxHalfCpltFlag = 0;

					BSP_AUDIO_OUT_Resume();
				}

				if (TxHalfCpltFlag == 1) {
					if (num > 2048)
						i = 2048;
					else
						i = num;

					if (dat != NULL) {
						memcpy((unsigned char*)audio_output_buffer, (unsigned char*)dat_addr, i * 2);
						dat_addr = dat_addr + i * 2;
					}

					// ����������ת��Ϊ������
					Audio_Convert_Mono(audio_output_buffer, i * 2 / 2);

					FrameNum += i;
					num = num - i;

					TxHalfCpltFlag = 0;

					BSP_AUDIO_OUT_Resume();
				}
			}
		}

		while (AudioOutFlag == 1) {
			if (Audio_ErrorFlag == 1) {
				// printf("��������>> ��Ƶ���Ź����г��ִ���!!!\r\n");
				BSP_AUDIO_OUT_Stop(0);
				return 0;
			}

			ES8388_Delay_1ms(4);
		}
	}

	BSP_AUDIO_OUT_Stop(0);

	// printf("Audio Play ok\r\n");

	return 1;
}

/**
 * @brief  ����¼��
 *	@param  wav_file: ¼���ļ���ŵ�����
 * @param  AudioFreq: ¼���ļ��Ĳ�����
 * @retval 1���ɹ� 0��ʧ��
 */
unsigned char AudioRecordStart(char* wav_file, int AudioFreq) {
	unsigned char buffer[35] = "Record/";
	unsigned int bw, temp;
	unsigned int ms;

	if (f_open(&file_object, (const TCHAR*)wav_file, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
		// printf("Create Record File Err!!!\r\n");
		return 0;
	}

	f_close(&file_object);

	if (f_open(&file_object, (const TCHAR*)wav_file, FA_OPEN_EXISTING | FA_WRITE) != FR_OK) {
		// printf("Open Record File Err!!!\r\n");
		return 0;
	}

	if (f_write(&file_object, (unsigned char*)"RIFF", 4, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	buffer[0] = 0;
	buffer[1] = 0;
	buffer[2] = 0;
	buffer[3] = 0;
	if (f_write(&file_object, buffer, 4, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	if (f_write(&file_object, (unsigned char*)"WAVE", 4, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	if (f_write(&file_object, (unsigned char*)"fmt ", 4, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	buffer[0] = 0x10;
	buffer[1] = 0;
	buffer[2] = 0;
	buffer[3] = 0;
	if (f_write(&file_object, buffer, 4, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	buffer[0] = 0x01;
	buffer[1] = 0;
	if (f_write(&file_object, buffer, 2, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	buffer[0] = 0x02;
	buffer[1] = 0;
	if (f_write(&file_object, buffer, 2, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	buffer[3] = AudioFreq >> 24;
	buffer[2] = (AudioFreq >> 16) & 0x00ff;
	buffer[1] = (AudioFreq >> 8) & 0x0000ff;
	buffer[0] = AudioFreq & 0x000000ff;
	if (f_write(&file_object, buffer, 4, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	temp = AudioFreq;
	temp = temp * 2 * 2;

	buffer[3] = temp >> 24;
	buffer[2] = (temp >> 16) & 0x00ff;
	buffer[1] = (temp >> 8) & 0x0000ff;
	buffer[0] = temp & 0x000000ff;
	if (f_write(&file_object, buffer, 4, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	buffer[0] = 0x04;
	buffer[1] = 0;
	if (f_write(&file_object, buffer, 2, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	buffer[0] = 0x10;
	buffer[1] = 0;
	if (f_write(&file_object, buffer, 2, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	if (f_write(&file_object, (unsigned char*)"data", 4, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	buffer[0] = 0;
	buffer[1] = 0;
	buffer[2] = 0;
	buffer[3] = 0;
	if (f_write(&file_object, buffer, 4, &bw) != FR_OK) {
		f_close(&file_object);
		return 0;
	}

	RxCpltFlag = 0;
	RxHalfCpltFlag = 0;
	Audio_ErrorFlag = 0;
	RecordFlag = 1;
	temp = 0;

	if (es8388_init(AUDIO_HAL_DAC_OUTPUT_NONE, AUDIO_HAL_ADC_INPUT_LINE1, AUDIO_HAL_MODE_SLAVE) == 1) {
		// printf("ES8388 Init ERR!!!\r\n");
		f_close(&file_object);
		es8388_deinit();
		return 0;
	}

	if (es8388_config_i2s(ES_MODULE_ADC, ES_I2S_NORMAL, AUDIO_HAL_BIT_LENGTH_16BITS) == 1) {
		// printf("ES8388 Config I2S ERR!!!\r\n");
		f_close(&file_object);
		es8388_deinit();
		return 0;
	}

	if (es8388_start(ES_MODULE_ADC) == 1) {
		// printf("ES8388 Start ERR!!!\r\n");
		f_close(&file_object);
		es8388_deinit();
		return 0;
	}

	MCU_I2S2_TX_Init(AudioFreq);
	MCU_I2S2_RX_Init(AudioFreq);

	memset(audio_buffer, 0, 4096);
	BSP_AUDIO_OUT_Play((unsigned short int*)audio_buffer, 2048);
	BSP_AUDIO_IN_Record(audio_output_buffer, 4096);

	// printf("Audio Record Start\r\n");
	ms = HAL_GetTick();

	while (1) {
		if (Audio_ErrorFlag == 1) {
			BSP_AUDIO_IN_Stop(0);
			BSP_AUDIO_OUT_Stop(0);
			f_close(&file_object);
			// printf("Audio Record Err!!!\r\n");
			return 0;
		}

		// ¼��10�룬ʱ�䵽ֹͣ¼��
		if ((HAL_GetTick() - ms) >= 10000) {
			RecordFlag = 0;
			// printf("Audio Recard Timeout\r\n");
		}

		if (RxCpltFlag == 1) {
			// printf("h %d\r\n", HAL_GetTick() - ms);
			//			ms = HAL_GetTick( );
			// printf("1\r\n");

			if (f_write(&file_object, (unsigned char*)(audio_output_buffer + 2048), 4096, &bw) != FR_OK) {
				f_close(&file_object);
				BSP_AUDIO_IN_Stop(0);
				BSP_AUDIO_OUT_Stop(0);
				return 0;
			}

			temp += bw;
			if (bw != 4096)
				// printf("err\r\n");
				RxCpltFlag = 0;
		}

		if (RxHalfCpltFlag == 1) {
			// printf("c %d\r\n", HAL_GetTick() - ms);
			//			ms = HAL_GetTick( );
			// printf("2\r\n");

			if (f_write(&file_object, (unsigned char*)audio_output_buffer, 4096, &bw) != FR_OK) {
				f_close(&file_object);
				BSP_AUDIO_IN_Stop(0);
				BSP_AUDIO_OUT_Stop(0);
				return 0;
			}
			temp += bw;
			if (bw != 4096)
				// printf("err\r\n");
				RxHalfCpltFlag = 0;
		}

		// ¼������
		if (RecordFlag == 0) {
			BSP_AUDIO_IN_Stop(0);
			BSP_AUDIO_OUT_Stop(0);

			buffer[3] = temp >> 24;
			buffer[2] = (temp >> 16) & 0x00ff;
			buffer[1] = (temp >> 8) & 0x0000ff;
			buffer[0] = temp & 0x000000ff;

			temp = f_tell(&file_object);

			f_lseek(&file_object, 40);

			if (f_write(&file_object, buffer, 4, &bw) != FR_OK) {
				f_close(&file_object);
				return 0;
			}

			buffer[3] = (temp - 8) >> 24;
			buffer[2] = ((temp - 8) >> 16) & 0x00ff;
			buffer[1] = ((temp - 8) >> 8) & 0x0000ff;
			buffer[0] = (temp - 8) & 0x000000ff;

			f_lseek(&file_object, 4);

			if (f_write(&file_object, buffer, 4, &bw) != FR_OK) {
				f_close(&file_object);
				return 0;
			}

			f_close(&file_object);

			// printf("Audio Recard ok\r\n");

			return 1;
		}

		ES8388_Delay_1ms(3);
	}
}

/**
 * @brief ����¼��
 *	@param  none
 * @retval none
 */
void AudioRecordEnd(void) {
	if (RecordFlag == 1)
		RecordFlag = 0;
}

void ES8388_Test(void) {
	es8388_deinit();	   // ��λES8388
	ES8388_Delay_1ms(100); // ��ʱ100����

	es8388_read_all(); // ��ӡES8388���мĴ�����ֵ

	ES8388_GPIO_Config(); // I2S���Žӿ�����

	/********************************������Ƶ********************************/
	// ѡ��DAC���ͨ��1(LOUT1\ROUT1)���ر�ADC����ͨ��������ES8388�����ڴӻ�ģʽ
	if (es8388_init(AUDIO_HAL_DAC_OUTPUT_LINE1, AUDIO_HAL_ADC_INPUT_NONE, AUDIO_HAL_MODE_SLAVE) == 1) {
		// printf("ES8388 Init ERR!!!\r\n");
		return;
	}

	// ����ES8388��I2Sͨ��Э��ΪI2S����Ƶ����λΪ16λ
	if (es8388_config_i2s(ES_MODULE_DAC, ES_I2S_NORMAL, AUDIO_HAL_BIT_LENGTH_16BITS) == 1) {
		// printf("ES8388 Config I2S ERR!!!\r\n");
		return;
	}

	// ������Ƶ����������С
	if (es8388_set_voice_volume(AUDIO_HAL_DAC_OUTPUT_LINE1, 80) == 1) {
		// printf("ES8388 Ser Voice ERR!!!\r\n");
		return;
	}

	// ����ES8388
	if (es8388_start(ES_MODULE_DAC) == 1) {
		// printf("ES8388 Start ERR!!!\r\n");
		return;
	}

	// ����I2S����ģʽ
	MCU_I2S2_TX_Init(16000);
	// // ����PCM��ʽ��Ƶ���ݣ�������16K����������
	play_pcm((unsigned char*)_acvolice1, 102528);
	ES8388_Delay_1ms(2000);

	// /********************************¼��********************************/
	// // ��ʼ¼������Ƶ�����ʣ�16000������λ��16
	// AudioRecordStart("MyFile.wav", 16000);
}
