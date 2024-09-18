#include "stm32f4xx_hal.h"
#include "wifi_module.h"  // Заголовок для работы с Wi-Fi
#include "gyro.h" // Заголовок для работы с гироскопом

I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1;
uint8_t gyro_data[2];

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void MX_USART1_UART_Init(void);
void ControlServos(uint8_t roll, uint8_t pitch);
void SendDataToWiFi(uint8_t* data);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();

    // Инициализация гироскопа
    InitGyro(&hi2c1); // Инициализируем гироскоп

    while (1) {
        // Чтение данных с гироскопа
        ReadGyroData(&hi2c1, gyro_data);

        // Управление сервоприводами на основе гироскопа
        ControlServos(gyro_data[0], gyro_data[1]);

        // Отправка данных на Wi-Fi
        SendDataToWiFi(gyro_data);
        HAL_Delay(100);
    }
}

// Реализация функций управления
void ControlServos(uint8_t roll, uint8_t pitch) {
    // Реализация управления сервоприводами
}

void SendDataToWiFi(uint8_t* data) {
    // Реализация отправки данных по Wi-Fi
}
