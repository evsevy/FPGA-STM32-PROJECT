#ifndef GYRO_H
#define GYRO_H

#include "stm32f4xx_hal.h"

#define GYRO_ADDRESS (0x68 << 1) // Адрес гироскопа

void InitGyro(I2C_HandleTypeDef* hi2c);
void ReadGyroData(I2C_HandleTypeDef* hi2c, uint8_t* data);

#endif // GYRO_H