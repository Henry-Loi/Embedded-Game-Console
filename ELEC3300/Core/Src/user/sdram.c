#include "sdram.h"

static int SDRAM_SendCommand(uint32_t CommandMode, uint32_t Bank, uint32_t RefreshNum, uint32_t RegVal)

{
	uint32_t CommandTarget;

	FMC_SDRAM_CommandTypeDef Command;

	if (Bank == 1) {
		CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;

	} else if (Bank == 2) {
		CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
	}

	Command.CommandMode = CommandMode;
	Command.CommandTarget = CommandTarget;
	Command.AutoRefreshNumber = RefreshNum;
	Command.ModeRegisterDefinition = RegVal;

	if (HAL_SDRAM_SendCommand(&hsdram1, &Command, 0x1000) != HAL_OK) {
		return -1;
	}

	return 0;
}


void SDRAM_Init(void) {
	uint32_t temp;

	// enable clock to sync
	SDRAM_SendCommand(FMC_SDRAM_CMD_CLK_ENABLE, 1, 1, 0);
	HAL_Delay(1);
	// precharge sdram to prepare for read/write
	SDRAM_SendCommand(FMC_SDRAM_CMD_PALL, 1, 1, 0);
	// enable autorefresh
	SDRAM_SendCommand(FMC_SDRAM_CMD_AUTOREFRESH_MODE, 1, 8, 0);

	// configure sdram mode register
	temp = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_3
		   | SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	SDRAM_SendCommand(FMC_SDRAM_CMD_LOAD_MODE, 1, 1, temp);

	HAL_SDRAM_ProgramRefreshRate(&hsdram1, 1668);
}

// Alternate SDRAM (UNUSED)
// uint8_t sdram_send_cmd(uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval) {
// 	uint32_t target_bank = 0;
// 	FMC_SDRAM_CommandTypeDef command;

// 	if (bankx == 0) {
// 		target_bank = FMC_SDRAM_CMD_TARGET_BANK1;
// 	} else if (bankx == 1) {
// 		target_bank = FMC_SDRAM_CMD_TARGET_BANK2;
// 	}

// 	command.CommandMode = cmd;
// 	command.CommandTarget = target_bank;
// 	command.AutoRefreshNumber = refresh;
// 	command.ModeRegisterDefinition = regval;

// 	return (HAL_SDRAM_SendCommand(&hsdram1, &command, 0X1000) != HAL_OK);
// }
// void sdram_initialization_sequence(void) {
// 	uint32_t temp = 0;

// 	sdram_send_cmd(0, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0);
// 	delay_us(500);
// 	sdram_send_cmd(0, FMC_SDRAM_CMD_PALL, 1, 0);
// 	sdram_send_cmd(0, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0);

// 	temp = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_3
// 		   | SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
// 	sdram_send_cmd(0, FMC_SDRAM_CMD_LOAD_MODE, 1, temp);
// }