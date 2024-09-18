# README: Создание системы управления с использованием гироскопа на STM32 и FPGA

## Описание проекта

Этот проект подробно описывает создание системы управления с использованием гироскопа, микроконтроллера STM32 и FPGA. Система предназначена для обработки данных гироскопа (IMU) и управления сервоприводами, а также для передачи информации по Wi-Fi.

## Содержание

1. [Аппаратная часть](#аппаратная-часть)
   - [Выбор компонентов](#выбор-компонентов)
   - [Схема подключения](#схема-подключения)
   - [Структура файлов](#структура-файлов)
2. [Программирование FPGA на VHDL](#программирование-fpga-на-vhdl)
3. [Программирование STM32 на C](#программирование-stm32-на-c)
   - [Основная программа на C](#основная-программа-на-c)
   - [Работа с гироскопом](#работа-с-гироскопом)
4. [Передача данных по Wi-Fi](#передача-данных-по-wi-fi)
5. [Сохранение данных на флеш-память](#сохранение-данных-на-флеш-память)
6. [Инструкции по сборке и настройке](#инструкции-по-сборке-и-настройке)
7. [Дополнительные улучшения](#дополнительные-uluchsheniya)
8. [Лицензия](#лицензия)

## Аппаратная часть

### Выбор компонентов

Для создания системы используйте следующие компоненты:
- **Гироскоп**: MPU-6050 (или аналогичный IMU).
- **STM32**: STM32F4 (или аналог).
- **FPGA**: Xilinx Spartan 7 или Intel DE0-CV.
- **Wi-Fi модуль**: ESP8266 для передачи данных.

### Схема подключения

1. **Гироскоп**: Подключите MPU-6050 к STM32 через I2C.
2. **STM32 и FPGA**: Соедините их через SPI или используйте GPIO для передачи данных.
3. **Wi-Fi модуль**: Подключите ESP8266 к STM32.

```
               +---------------------+
               |        STM32       |
               |                     |
               |          I2C       |<--- MPU-6050
               |                     |
               |          SPI       |<--- FPGA
               |                     |    
               |         UART       |<--- ESP8266
               +---------------------+
```

### Структура файлов

Для упрощения работы с проектом, используйте следующую структуру файлов:

```
/ProjectRoot
├── /FPGA
│   ├── GyroProcessor.vhd
│   └── constraints.xdc
├── /STM32
│   ├── main.c
│   ├── gyro.h
│   ├── gyro.c
│   ├── wifi_module.h
│   └── wifi_module.c
├── /Documentation
│   ├── README.md
│   └── LICENSE
└── /Testing
    └── test_gyro.c
```

## Программирование FPGA на VHDL

Создайте VHDL код для обработки данных с гироскопа и управления сервоприводами.

```vhdl
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity GyroProcessor is
    port (
        clk           : in std_logic;
        reset         : in std_logic;
        gyro_data     : in std_logic_vector(15 downto 0);
        servo_roll    : out std_logic_vector(15 downto 0);
        servo_pitch   : out std_logic_vector(15 downto 0)
    );
end entity GyroProcessor;

architecture Behavioral of GyroProcessor is
begin
    process(clk, reset)
    begin
        if reset = '1' then
            servo_roll <= (others => '0');
            servo_pitch <= (others => '0');
        elsif rising_edge(clk) then
            -- Обработка данных гироскопа
            servo_roll <= gyro_data; -- Здесь можно добавить фильтрацию
            servo_pitch <= gyro_data; -- Здесь тоже
        end if;
    end process;
end architecture Behavioral;
```

## Программирование STM32 на C

Теперь напишем C-код для работы с гироскопом и управления сервоприводами.

### Основная программа на C

```c
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
```

### Работа с гироскопом

Создайте файл `gyro.h` и `gyro.c`, которые будут содержать функции инициализации гироскопа и считывания данных:

```c
// gyro.h
#ifndef GYRO_H
#define GYRO_H

#include "stm32f4xx_hal.h"

#define GYRO_ADDRESS (0x68 << 1) // Адрес гироскопа

void InitGyro(I2C_HandleTypeDef* hi2c);
void ReadGyroData(I2C_HandleTypeDef* hi2c, uint8_t* data);

#endif // GYRO_H
```

```c
// gyro.c
#include "gyro.h"

void InitGyro(I2C_HandleTypeDef* hi2c) {
    // Настройка гироскопа
    uint8_t data[2] = {0x6B, 0x00}; // Выключаем спящий режим
    HAL_I2C_Master_Transmit(hi2c, GYRO_ADDRESS, data, 2, HAL_MAX_DELAY);
}

void ReadGyroData(I2C_HandleTypeDef* hi2c, uint8_t* data) {
    HAL_I2C_Mem_Read(hi2c, GYRO_ADDRESS, 0x43, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
}
```

## Передача данных по Wi-Fi

Используйте встроенные функции для работы с вашим Wi-Fi модулем (например, ESP8266) для передачи данных, используя HTTP или другой протокол.

```c
void SendDataToWiFi(uint8_t* data) {
    // Пример кода для отправки данных на сервер
    char message[50];
    sprintf(message, "roll=%d&pitch=%d", data[0], data[1]);
    // Код для отправки "message" по Wi-Fi
}
```

## Сохранение данных на флеш-память

Используйте HAL для работы с флеш-памятью STM32:

```c
void SaveDataToFlash(uint8_t* data) {
    // Код для записи данных на флеш-память
}
```

## Инструкции по сборке и настройке

1. **Сборка схемы**:
   - Подключите все компоненты согласно схеме.

2. **Загрузка кода**:
   - Загрузите прошивку в STM32 с использованием STM32CubeProgrammer.
   - Скомпилируйте и загрузите VHDL-код в FPGA с использованием Vivado или Quartus.

3. **Настройка Wi-Fi**:
   - Настройте параметры подключения в коде микроконтроллера.

4. **Тестирование**:
   - Проверьте работоспособность системы, считывание данных и управление сервоприводами.

## Дополнительные улучшения

1. **Обработка ошибок**: Добавьте обработку ошибок в функции работы с гироскопом и Wi-Fi.
2. **Оптимизация**: Убедитесь, что используете оптимизированные функции для обработки данных в реальном времени.

## Лицензия

Этот проект находится под лицензией MIT. Для получения дополнительной информации смотрите файл LICENSE.