/*
 * sdram.c
 *
 *  Created on: Oct 16, 2023
 *      Author: Areion
 */
#include "sdram.h"

#include "stm32f4xx_hal_sdram.h"

static SDRAM_HandleTypeDef sdramHandle;
static FMC_SDRAM_CommandTypeDef Command;

/******************************************************************************
function:	SDRAM device configuration
******************************************************************************/
// void SDRAM_Init(void)
// {
// 	sdramHandle.Instance = FMC_SDRAM_DEVICE;
// 	SDRAM_Initialization_sequence(REFRESH_COUNT);
// }
void SDRAM_Init(void) {
	FMC_SDRAM_TimingTypeDef sdram_timing;

	sdramHandle.Instance = FMC_SDRAM_DEVICE;							   /* SDRAM��BANK5,6 */
	sdramHandle.Init.SDBank = FMC_SDRAM_BANK1;							   /* ��һ��SDRAM BANK */
	sdramHandle.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;	   /* ������ */
	sdramHandle.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;			   /* ������ */
	sdramHandle.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;		   /* ���ݿ���Ϊ16λ */
	sdramHandle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;	   /* һ��4��BANK */
	sdramHandle.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;				   /* CASΪ2 */
	sdramHandle.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE; /* ʧ��д���� */
	sdramHandle.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;			   /* SDRAMʱ��ΪHCLK/2=192M/2=96M=10.4ns */
	sdramHandle.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;				   /* ʹ��ͻ�� */
	sdramHandle.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;			   /* ��ͨ����ʱ */

	sdram_timing.LoadToActiveDelay = 2; /* ����ģʽ�Ĵ���������ʱ����ӳ�Ϊ2��ʱ������ */
	sdram_timing.ExitSelfRefreshDelay = 7; /* �˳���ˢ���ӳ�Ϊ7��ʱ������ */
	sdram_timing.SelfRefreshTime = 6;	   /* ��ˢ��ʱ��Ϊ6��ʱ������ */
	sdram_timing.RowCycleDelay = 6;		   /* ��ѭ���ӳ�Ϊ6��ʱ������ */
	sdram_timing.WriteRecoveryTime = 2;	   /* �ָ��ӳ�Ϊ2��ʱ������ */
	sdram_timing.RPDelay = 2;			   /* ��Ԥ����ӳ�Ϊ2��ʱ������ */
	sdram_timing.RCDDelay = 2;			   /* �е����ӳ�Ϊ2��ʱ������ */
	HAL_SDRAM_Init(&sdramHandle, &sdram_timing);

	// sdram_initialization_sequence(); /* ����SDRAM��ʼ������ */

	/* ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
	 * COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
	 * ����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=192/2=96Mhz,����Ϊ8192(2^13).
	 * ����,COUNT=64*1000*96/8192-20=730 */
	HAL_SDRAM_ProgramRefreshRate(&sdramHandle, 730); /* ����ˢ��Ƶ�� */
}


/******************************************************************************
function:	Programs the SDRAM device.
parameter:
	RefreshCount : SDRAM refresh counter value
******************************************************************************/
void SDRAM_Initialization_sequence(uint32_t RefreshCount) {
	__IO uint32_t tmpmrd = 0;

	/* Step 1: Configure a clock configuration enable command */
	Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 2: Insert 100 us minimum delay */
	/* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
	HAL_Delay(1);

	/* Step 3: Configure a PALL (precharge all) command */
	Command.CommandMode = FMC_SDRAM_CMD_PALL;
	Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 4: Configure an Auto Refresh command */
	Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber = 8;
	Command.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 5: Program the external memory mode register */
	tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_3
			 | SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = tmpmrd;

	/* Send the command */
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

	/* Step 6: Set the refresh rate counter */
	/* Set the device refresh rate */
	HAL_SDRAM_ProgramRefreshRate(&sdramHandle, RefreshCount);
}
