#include "gyro.h"

void InitGyro(I2C_HandleTypeDef* hi2c) {
    // Настройка гироскопа
    uint8_t data[2] = {0x6B, 0x00}; // Выключаем спящий режим
    HAL_I2C_Master_Transmit(hi2c, GYRO_ADDRESS, data, 2, HAL_MAX_DELAY);
}

void ReadGyroData(I2C_HandleTypeDef* hi2c, uint8_t* data) {
    HAL_I2C_Mem_Read(hi2c, GYRO_ADDRESS, 0x43, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
}