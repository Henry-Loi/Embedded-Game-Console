#ifndef _ICM20602_H_
#define _ICM20602_H_
#include "main.h"
#include "stdint.h"

#include "stm32f4xx_hal.h"

#define ICM20_ACCEL_FS_2G  (0 << 3)
#define ICM20_ACCEL_FS_4G  (1 << 3)
#define ICM20_ACCEL_FS_8G  (2 << 3)
#define ICM20_ACCEL_FS_16G (3 << 3)

#define ICM20_GYRO_FS_250  (0 << 3)
#define ICM20_GYRO_FS_500  (1 << 3)
#define ICM20_GYRO_FS_1000 (2 << 3)
#define ICM20_GYRO_FS_2000 (3 << 3)

#define DLPF_BW_250 0x00
#define DLPF_BW_176 0x01
#define DLPF_BW_92	0x02
#define DLPF_BW_41	0x03
#define DLPF_BW_20	0x04
#define DLPF_BW_10	0x05
#define DLPF_BW_5	0x06
#define DLPF_BW_328 0x06

#define ACCEL_AVER_4  (0x00 << 4)
#define ACCEL_AVER_8  (0x01 << 4)
#define ACCEL_AVER_16 (0x02 << 4)
#define ACCEL_AVER_32 (0x03 << 4)

#define ACCEL_DLPF_BW_218 0x00
#define ACCEL_DLPF_BW_218 0x01
#define ACCEL_DLPF_BW_99  0x02
#define ACCEL_DLPF_BW_44  0x03
#define ACCEL_DLPF_BW_21  0x04
#define ACCEL_DLPF_BW_10  0x05
#define ACCEL_DLPF_BW_5	  0x06
#define ACCEL_DLPF_BW_420 0x06


uint8_t icm20602_init(void);

uint8_t icm20602_set_gyro_fullscale(uint8_t fs);
uint8_t icm20602_set_accel_fullscale(uint8_t fs);
uint8_t icm20602_get_accel_adc(int16_t* accel);
uint8_t icm20602_get_gyro_adc(int16_t* gyro);
uint8_t icm20602_get_gyro(float* gyro);
uint8_t icm20602_get_accel(float* accel);
float icm20602_get_temp(void);

typedef struct {
	int16_t accel[3];
	int16_t gyro[3];
	int16_t gyro_last[3];
	float gyro_vel[3];
	float gyro_offset[3];
} IMUData;

void imu_thread(void* par);
int imu_tft(int r);

extern IMUData imu_data;

#endif