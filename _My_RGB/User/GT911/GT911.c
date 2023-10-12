/*****************************************************************************
* | File      	:   GT911.c
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
#include "GT911.h"


UWORD x[CT_MAX_TOUCH] = {0}, y[CT_MAX_TOUCH] = {0}, z=0;
UWORD Color[CT_MAX_TOUCH] = {BLACK, BRED, BLUE, RED, BROWN};
UWORD temp=0;
GT911_Dev Dev_Now,Dev_Backup;

/******************************************************************************
function:	GT911 reset

GT9147 �ϵ�ĵ�ʱ��INT��������Ǹߵ�ƽ��ô��0x14Ϊ��ַ������ǵ͵�ƽ����0x5DΪ��ַ
******************************************************************************/
static void GT911_Reset()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/*����INT������������*/
	DEV_Set_GPIOMode(DEV_INT_PIN, 0);
	
	/*INT��������*/
	HAL_GPIO_WritePin(DEV_INT_PIN, GPIO_PIN_SET);
	HAL_Delay(1);
	
	/*RES��������*/
	HAL_GPIO_WritePin(DEV_RST_PIN, GPIO_PIN_RESET);	
	HAL_Delay(10);
	
	/*RES�������� ���Ӳ����λ*/
	HAL_GPIO_WritePin(DEV_RST_PIN, GPIO_PIN_SET); 
	HAL_Delay(10);
    
	/*��������INT����Ϊ����ģʽ*/
  GPIO_InitStruct.Pin=INT_Pin;            //PH7
  GPIO_InitStruct.Mode=GPIO_MODE_INPUT;      //����
  GPIO_InitStruct.Pull=GPIO_NOPULL;          //��������������������
  GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     //����
  HAL_GPIO_Init(INT_GPIO_Port,&GPIO_InitStruct);     //��ʼ��
	DEV_Delay_ms(100);
}

/******************************************************************************
function:	Write data to the GT911 register
parameter:
    reg   :   Register address
		buf		��	Data to send
		len		��	Length of data sent
******************************************************************************/
static uint8_t GT911_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	I2C_Start();	
  I2C_Send_Byte(CT_CMD_WR);   
	I2C_Wait_Ack();
	I2C_Send_Byte(reg>>8);   	
	I2C_Wait_Ack(); 	 										  		   
	I2C_Send_Byte(reg&0XFF);   	
	I2C_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    I2C_Send_Byte(buf[i]);  
		ret=I2C_Wait_Ack();
		if(ret)break;  
	}
  I2C_Stop();					
	return ret; 
}

/******************************************************************************
function:	Read data from the GT911 register
parameter:
    reg   :   Register address
		buf		��	Read data
		len		��	Read the length of the data
******************************************************************************/
static void GT911_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	uint8_t i;
 	I2C_Start();	
 	I2C_Send_Byte(CT_CMD_WR);  
	I2C_Wait_Ack();
 	I2C_Send_Byte(reg>>8);   
	I2C_Wait_Ack(); 	 										  		   
 	I2C_Send_Byte(reg&0XFF);   	
	I2C_Wait_Ack();  
 	I2C_Stop();  
	
 	I2C_Start();  	 	   
	I2C_Send_Byte(CT_CMD_RD);     
	I2C_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
		buf[i]=I2C_Read_Byte(i==(len-1)?0:1); 
	} 
  I2C_Stop();   
}

/******************************************************************************
function:	Read the ID of the GT911
******************************************************************************/
static uint8_t GT911_Read_ID(void)
{
	uint8_t buf[4];
	uint8_t temp;
	GT911_RD_Reg(GT911_PRODUCT_ID_REG, (uint8_t *)&buf[0], 4);
	GT911_RD_Reg(GT911_CONFIG_REG, (uint8_t *)&temp, 1);
	printf("TouchPad_ID:%c%c%c%c\r\nTouchPad_Config_Version:%d\r\n",buf[0],buf[1],buf[2],buf[3],temp);
	return temp;
}

/******************************************************************************
function:	Read the firmware version of the GT911
******************************************************************************/
/*GT9147û��0x8044�Ĵ���*/
//static uint16_t GT911_Read_FirmwareVersion(void)
//{
//	uint8_t buf[2];

//	GT911_RD_Reg(GT911_FIRMWARE_VERSION_REG, buf, 2);

//	printf("FirmwareVersion:%2x\r\n",(((uint16_t)buf[1] << 8) + buf[0]));
//	return ((uint16_t)buf[1] << 8) + buf[0];
//}


const uint8_t GT9147_CFG_TBL[]=
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
};  
//����GT9147���ò���
//mode:0,���������浽flash
//     1,�������浽flash
static uint8_t GT9147_Send_Cfg(uint8_t mode)
{
	uint8_t buf[2];
	uint8_t i=0;
	buf[0]=0;
	buf[1]=mode;	//�Ƿ�д�뵽GT9147 FLASH?  ���Ƿ���籣��
	for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i];//����У���
    buf[0]=(~buf[0])+1;
	GT911_WR_Reg(GT911_CONFIG_REG,(uint8_t*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//���ͼĴ�������
	GT911_WR_Reg(GT_CHECK_REG,buf,2);//д��У���,�����ø��±��
	return 0;
} 

/******************************************************************************
function:	���ݴ��������Ժ���
******************************************************************************/

const uint16_t POINT_COLOR_TBL[10]={RED,GREEN,BLUE,BROWN,GRED,BRED,GBLUE,LIGHTBLUE,BRRED,GRAY}; 

void ctp_test(void)
{
	uint8_t t=0;  	    
 	uint16_t lastpos[10][2];		//���һ�ε����� 
	uint8_t maxp=5;
	while(1)
	{
		GT911_Scan(0);
		for(t=0;t<maxp;t++)
		{
			if((Dev_Now.Touch)&(1<<t))
			{
				if(Dev_Now.X[t]<LCD_WIDTH&&Dev_Now.Y[t]<LCD_HEIGHT)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = Dev_Now.X[t];
						lastpos[t][1] = Dev_Now.Y[t];
					}
					Paint_DrawLine(lastpos[t][0],lastpos[t][1],Dev_Now.X[t],Dev_Now.Y[t], POINT_COLOR_TBL[t], LINE_STYLE_SOLID, DOT_PIXEL_2X2);
					lastpos[t][0]=Dev_Now.X[t];
					lastpos[t][1]=Dev_Now.Y[t];
					if(Dev_Now.X[t]>(715)&&Dev_Now.Y[t]<24)
					{
						Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, ROTATE_0, WHITE);
						Paint_Clear(WHITE);
						Paint_DrawString_EN(715, 0, "clear", &Font24, BLUE, CYAN);/*ÿ���ַ�ռ17��x���� 24��Y����*/
					}
				}
			}else lastpos[t][0]=0XFFFF;
		} 
		
		HAL_Delay(5);
	}
}
/******************************************************************************
function:	������ɨ�躯��
******************************************************************************/
const uint16_t GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

uint8_t GT911_Scan(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint8_t temp;
	uint8_t tempsta;
 	static uint8_t t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	t++;
	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
	{
		GT911_RD_Reg(GT_GSTID_REG,&mode,1);	//��ȡ�������״̬  
 		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;
			GT911_WR_Reg(GT_GSTID_REG,&temp,1);//���־ 		
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			tempsta=Dev_Now.Touch;			//���浱ǰ��tp_dev.staֵ
			Dev_Now.Touch=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			Dev_Now.X[4]=Dev_Now.X[0];	//���津��0������
			Dev_Now.Y[4]=Dev_Now.Y[0];
			for(i=0;i<5;i++)
			{
				if(Dev_Now.Touch&(1<<i))	//������Ч?
				{
					GT911_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
                    if(1)   //4.3��800*480 MCU��
                    {
                        if(1)//����
                        {
                           Dev_Now.Y[i]=480-(((uint16_t)buf[1]<<8)+buf[0]);
													
													
                           Dev_Now.X[i]=800-(((uint16_t)buf[3]<<8)+buf[2]);//
                        }else//����
                        {
                            Dev_Now.X[i]=((uint16_t)buf[1]<<8)+buf[0];
                            Dev_Now.Y[i]=((uint16_t)buf[3]<<8)+buf[2];
                        }  
                    }
//										else if(lcddev.id==0X4342) //4.3��480*272 RGB��
//                    {
//                        if(tp_dev.touchtype&0X01)//����
//                        {
//                            tp_dev.x[i]=(((u16)buf[1]<<8)+buf[0]);
//                            tp_dev.y[i]=(((u16)buf[3]<<8)+buf[2]);
//                        }else
//                        {
//                            tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
//                            tp_dev.x[i]=272-(((u16)buf[3]<<8)+buf[2]);
//                        }
//                    } 
					printf("x[%d]:%d,y[%d]:%d\r\n",i,Dev_Now.X[i],i,Dev_Now.Y[i]);
				}			
			} 
			res=1;
			if(Dev_Now.X[0]>LCD_WIDTH||Dev_Now.Y[0]>LCD_HEIGHT)//�Ƿ�����(���곬����)
			{ 
				if((mode&0XF)>1)		//��������������,�򸴵ڶ�����������ݵ���һ������.
				{
					Dev_Now.X[0]=Dev_Now.X[1];
					Dev_Now.Y[0]=Dev_Now.Y[1];
					t=0;				//����һ��,��������������10��,�Ӷ����������
				}else					//�Ƿ�����,����Դ˴�����(��ԭԭ����)  
				{
					Dev_Now.X[0]=Dev_Now.X[4];
					Dev_Now.Y[0]=Dev_Now.Y[4];
					mode=0X80;		
					Dev_Now.Touch=tempsta;	//�ָ�tp_dev.sta
				}
			}else t=0;					//����һ��,��������������10��,�Ӷ����������
		}
	}
	if((mode&0X8F)==0X80)//�޴����㰴��
	{ 
		if(Dev_Now.Touch&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			Dev_Now.Touch&=~(1<<7);	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			Dev_Now.X[0]=0xffff;
			Dev_Now.Y[0]=0xffff;
			Dev_Now.Touch&=0XE0;	//�������Ч���	
		}	 
	} 	
	if(t>240)t=10;//���´�10��ʼ����
	return res;
}

/******************************************************************************
function:GT9147оƬ��ʼ��
******************************************************************************/
void GT911_Init(void)
{
	uint8_t temp = 0;
	uint8_t Config_Version = 0;
	
	printf("GT911_Init\r\n");
	
	/*���ô�����������*/
	Dev_Now.touchtype = 1;
	
	/*��������оƬ��λ*/
	GT911_Reset();
	
	/*��ȡ����оƬID�����ش���оƬ���ð汾*/
	Config_Version = GT911_Read_ID();
	
	/*�����λGT9147,*/
	temp=0X02;	
	GT911_WR_Reg(GT911_COMMAND_REG,	(uint8_t *)&temp,1);//��λGT9147
	
	/*�Ƿ���Ҫ��������GT9147*/
	if(Config_Version<0x60)//Ĭ�ϰ汾�Ƚϵ�,��Ҫ����flash����
	{
		printf("Default Ver:%d\r\n",Config_Version);
		GT9147_Send_Cfg(1);//���²���������
	}	
	
	/*������λ */ 
	HAL_Delay(10);	
	temp=0X00;	 
	GT911_WR_Reg(GT911_COMMAND_REG,(uint8_t *)&temp,1);

}




