#include "ens160.h"

#include "iot_i2c.h"
#include "iot_errno.h"

#include <stdio.h>

/* ens160 对应i2c */
#define ENS160_I2C_PORT EI2C0_M2

/* ens160 地址 */
#define ENS160_I2C_ADDRESS 0x52

/**
 * @brief ens160寄存器
 *
 */
typedef enum ens160_register {
    PART_ID       = 0x00,
    OPMODE        = 0X10,
    CONFIG        = 0X11,
    COMMAND       = 0X12,
    TEMP_IN       = 0X13,
    RH_IN         = 0X15,
    DEVICE_STATUS = 0X20,
    DATA_AQI      = 0X21,
    DATA_TVOC     = 0X22,
    DATA_ECO2     = 0X24,
    DATA_T        = 0X30,
    DATA_RH       = 0X32,
    DATAMISR      = 0X38,
    GPR_WRITE     = 0X40,
    GPR_READ      = 0X48,

} ens160_register_t;

/**
 * @brief ens160运行模式
 *
 */
typedef enum ens160_modes {
    DEEP_SLEEP_MODE = 0X00,
    IDLE_MODE,
    OPERATIONAL_MODE,
} ens160_modes_t;

/**
 * @brief ens160 初始化
 * 
 */
void ens160_init(void)
{
    uint32_t ret = IoTI2cInit(ENS160_I2C_PORT, EI2C_FRE_400K);
    if (ret != IOT_SUCCESS) {
        printf("IoTI2cInit failed, ret=%d", ret);
        return;
    }

    uint8_t init_data[] = {OPMODE, OPERATIONAL_MODE};
    IoTI2cWrite(ENS160_I2C_PORT, ENS160_I2C_ADDRESS, init_data, sizeof(init_data));
}

/**
 * @brief ens160 设置温度和湿度
 * 
 * @param temperature 温度
 * @param humidity 湿度
 */
void ens160_set_temperature_humidity(double temperature, double humidity)
{
    uint16_t int_temperature = (uint16_t)((temperature + 273.15) * 64);
    uint16_t int_humidity    = (uint16_t)(humidity * 512);

    uint8_t temperature_data[] = {TEMP_IN, (uint8_t)(int_temperature & 0xFF), (uint8_t)(int_temperature >> 8)};
    IoTI2cWrite(ENS160_I2C_PORT, ENS160_I2C_ADDRESS, temperature_data, sizeof(temperature_data));

    uint8_t humidity_data[] = {RH_IN, (uint8_t)(int_humidity & 0xFF), (uint8_t)(int_humidity >> 8)};
    IoTI2cWrite(ENS160_I2C_PORT, ENS160_I2C_ADDRESS, humidity_data, sizeof(humidity_data));
}

/**
 * @brief ens160获取数据
 * 
 * @param data 数据
 */
void ens160_get_data(uint16_t *data)
{
    uint8_t write_data[] = {DATA_ECO2};
    IoTI2cWrite(ENS160_I2C_PORT, ENS160_I2C_ADDRESS, write_data, sizeof(write_data));

    uint8_t read_data[2] = {};
    IoTI2cRead(ENS160_I2C_PORT, ENS160_I2C_ADDRESS, read_data, sizeof(read_data));
    uint16_t co2_value = (read_data[1]) << 8 | read_data[0];

    *data = co2_value;
}
