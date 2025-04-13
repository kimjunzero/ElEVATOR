#include <math.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "main.h"

#define BMP180_I2C 		&hi2c1
#define BMP180_ADDRESS 	(0x77 << 1)	// 7bit 주소를 왼쪽 1비트 shift
#define I2C_TIMEOUT_MS	100

void calib_data(void);
uint16_t read_uncom_temp (void);
float cal_temp (void);
uint32_t read_uncom_press (int oss);
float cal_preass(int oss);
