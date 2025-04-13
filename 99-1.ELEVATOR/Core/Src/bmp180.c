#include "bmp180.h"

// Defines by datasheet													// datasheet보고 수정하기
short AC1, AC2, AC3, B1, B2, MB, MC, MD;
unsigned short AC4, AC5, AC6;

extern I2C_HandleTypeDef hi2c1;

void calib_data(void)
{
	uint8_t Calib_Data[22] = {0,};
	uint16_t Calib_Start = 0xAA;

	if (HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, Calib_Start, 1, Calib_Data, 22, I2C_TIMEOUT_MS) != HAL_OK)
	{
		printf("I2C Read Timeout (Calibration)\r\n");
	}

	AC1 = ((Calib_Data[0] << 8) | Calib_Data[1]);
	AC2 = ((Calib_Data[2] << 8) | Calib_Data[3]);
	AC3 = ((Calib_Data[4] << 8) | Calib_Data[5]);
	AC4 = ((Calib_Data[6] << 8) | Calib_Data[7]);
	AC5 = ((Calib_Data[8] << 8) | Calib_Data[9]);
	AC6 = ((Calib_Data[10] << 8) | Calib_Data[11]);
	B1 = ((Calib_Data[12] << 8) | Calib_Data[13]);
	B2 = ((Calib_Data[14] << 8) | Calib_Data[15]);
	MB = ((Calib_Data[16] << 8) | Calib_Data[17]);
	MC = ((Calib_Data[18] << 8) | Calib_Data[19]);
	MD = ((Calib_Data[20] << 8) | Calib_Data[21]);
}

// Get uncompensated Temp
uint16_t read_uncom_temp (void)
{
	uint8_t datatowrite = 0x2E;
	uint8_t Temp_RAW[2] = {0};
	if (HAL_I2C_Mem_Write(BMP180_I2C, BMP180_ADDRESS, 0xF4, 1, &datatowrite, 1, I2C_TIMEOUT_MS) != HAL_OK)
	{
		printf("I2C Write Timeout (Temp)\r\n");
		return 0;
	}

	HAL_Delay (5);
	if (HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, 0xF6, 1, Temp_RAW, 2, I2C_TIMEOUT_MS) != HAL_OK)
	{
		printf("I2C Read Timeout (Temp)\r\n");
		return 0;
	}

	return ((Temp_RAW[0]<<8) + Temp_RAW[1]);
}

float cal_temp (void)
{
	long UT = read_uncom_temp();
	long X1 = ((UT - AC6) * AC5) >> 15;
	long X2 = (MC << 11) / (X1 + MD);
	long B5 = X1 + X2;
	long Temp = (B5 + 8) >> 4;

	return Temp / 10.0;
}

// Get uncompensated Pressure
uint32_t read_uncom_press (int oss)   // oversampling setting 0,1,2,3
{
	uint8_t cmd = 0x34 + (oss << 6);
	uint8_t Press_RAW[3] = {0};

	if (HAL_I2C_Mem_Write(BMP180_I2C, BMP180_ADDRESS, 0xF4, 1, &cmd, 1, I2C_TIMEOUT_MS) != HAL_OK)
	{
		printf("I2C Write Timeout (Press)\r\n");
		return 0;
	}

	switch (oss)
	{
		case 0: HAL_Delay(5); break;
		case 1: HAL_Delay(8); break;
		case 2: HAL_Delay(14); break;
		case 3: HAL_Delay(26); break;
	}

	if (HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, 0xF6, 1, Press_RAW, 3, I2C_TIMEOUT_MS) != HAL_OK)
	{
		printf("I2C Read Timeout (Press)\r\n");
		return 0;
	}

	return (((Press_RAW[0] << 16) + (Press_RAW[1] << 8) + Press_RAW[2]) >> (8 - oss));
}


float cal_preass(int oss)
{
	long UT = read_uncom_temp();
	long UP = read_uncom_press(oss);

	long X1 = ((UT - AC6) * AC5) >> 15;
	long X2 = (MC << 11) / (X1 + MD);
	long B5 = X1 + X2;
	long B6 = B5 - 4000;

	X1 = (B2 * ((B6 * B6) >> 12)) >> 11;
	X2 = (AC2 * B6) >> 11;
	long X3 = X1 + X2;
	long B3 = ((((long)AC1 * 4 + X3) << oss) + 2) >> 2;

	X1 = (AC3 * B6) >> 13;
	X2 = (B1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;

	unsigned long B4 = (AC4 * (unsigned long)(X3 + 32768)) >> 15;
	unsigned long B7 = ((unsigned long)UP - B3) * (50000 >> oss);

	long p;
	if (B7 < 0x80000000)
		p = (B7 << 1) / B4;
	else
		p = (B7 / B4) << 1;

	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	p = p + ((X1 + X2 + 3791) >> 4);

	return p / 100.0 / 1013.25;  // atm 단위(1atm = 1hPa/1013.25 )
}
