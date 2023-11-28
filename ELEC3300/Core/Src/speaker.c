#include "speaker.h"

#include "es8388.h"


void speaker_thread(void* par) {
	int do_once_flag = 0;
	I2C_Init();
	while (1) {
		osDelay(4);
		if (do_once_flag == 0) {
			do_once_flag = 1;
		}
		ES8388_Test();
	}
}
