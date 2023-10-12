/*****************************************************************************
* | File      	:   GT911.H
* | Author      :   Waveshare team
* | Function    :   GT911 driver
* | Info        :
*               GT911 driver and data processing
*----------------
* |	This version:   V1.0
* | Date        :   2019-3-5
* | Info        :

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __GT911_H_
#define __GT911_H_


#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "Analog_I2C.h"
#include "gpio.h"

#define GT911_MAX_WIDTH		1024    	//Touchscreen pad max width
#define GT911_MAX_HEIGHT		600			//Touchscreen pad max height

//#define CT_CMD_WR			0XBA					//Write data command
//#define CT_CMD_RD   	0XBB					//Read data command

#define CT_CMD_WR 		0X28     	//写命令
#define CT_CMD_RD 		0X29		//读命令

//The maximum number of points supported by the capacitive touch screen
#define CT_MAX_TOUCH    5						//电容屏支持的点数,固定为5点

#define GT911_COMMAND_REG   				0x8040	/* Real-time command */
#define GT911_CONFIG_REG						0x8047	/* Configuration parameter register */
#define GT911_PRODUCT_ID_REG 				0x8140 	/* Product ID */
//#define GT911_FIRMWARE_VERSION_REG  0x8144  /* Firmware version number */
#define GT911_READ_XY_REG 					0x814E	/* Coordinate register */
#define GT_CHECK_REG 								0X80FF   	/*GT9147校验和寄存器*/


#define GT_CTRL_REG 	0X8040   	//GT9147控制寄存器
#define GT_CFGS_REG 	0X8047   	//GT9147配置起始地址寄存器
#define GT_CHECK_REG 	0X80FF   	//GT9147校验和寄存器
#define GT_PID_REG 		0X8140   	//GT9147产品ID寄存器

#define GT_GSTID_REG 	0X814E   	//GT9147当前检测到的触摸情况
#define GT_TP1_REG 		0X8150  	//第一个触摸点数据地址
#define GT_TP2_REG 		0X8158		//第二个触摸点数据地址
#define GT_TP3_REG 		0X8160		//第三个触摸点数据地址
#define GT_TP4_REG 		0X8168		//第四个触摸点数据地址
#define GT_TP5_REG 		0X8170		//第五个触摸点数据地址 

#define TP_PRES_DOWN 0x80  			//触屏被按下	  
#define TP_CATH_PRES 0x40  			//有按键按下了 



typedef struct
{
	uint16_t Touch;				/*触摸的状态 
													b15:按下1/松开0; 
													b14:0,没有按键按下;1,有按键按下. 
													b13~b10:保留
													b9~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
												*/
	uint8_t	touchtype;		/*触摸屏的类型
													b1~6:保留.
													b7:0,电阻屏1,电容屏
												*/
	uint8_t TouchpointFlag;
	uint8_t TouchCount;
 
	uint8_t Touchkeytrackid[CT_MAX_TOUCH];
	uint16_t X[CT_MAX_TOUCH];
	uint16_t Y[CT_MAX_TOUCH];
	uint16_t S[CT_MAX_TOUCH];
}GT911_Dev;

void GT911_Init(void);
uint8_t GT911_Scan(uint8_t mode);
void ctp_test(void);
#endif /*__GT911_H_*/
