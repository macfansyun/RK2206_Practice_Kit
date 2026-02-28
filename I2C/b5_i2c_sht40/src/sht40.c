#include "sht40.h"

#include "iot_i2c.h"
#include "iot_errno.h"

/* sht40对应i2c */
#define SHT40_I2C_PORT EI2C0_M2

/* sht40地址 */
#define SHT40_I2C_ADDRESS 0x44

/**
 * @brief 延时函数
 *
 * @param ms 延时ms
 */
static void delay_ms(uint32_t ms)
{
    HAL_DelayMs(ms);
}

/**
 * @brief 检查数据正确性
 *
 * @param data 读取到的数据
 * @param nbrOfBytes 需要校验的数量
 * @param checksum 读取到的校对比验值
 * @return true 成功
 * @return false 失败
 */
static bool sht40_check_crc(uint8_t *data, uint8_t nbrOfBytes, uint8_t checksum)
{
    uint8_t crc = 0xFF;
    uint8_t bit = 0;
    uint8_t byteCtr;
    const int16_t POLYNOMIAL = 0x131;

    /*calculates 8-Bit checksum with given polynomial*/
    for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr) {
        crc ^= (data[byteCtr]);
        for (bit = 8; bit > 0; --bit) {
            if (crc & 0x80)
                crc = (crc << 1) ^ POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }

    if (crc != checksum)
        return false;
    else
        return true;
}

/**
 * @brief 温度计算
 *
 * @param u16sT 读取到的温度原始数据
 * @return float 计算后的温度数据
 */
static float sht40_calc_temperature(uint16_t u16sT)
{
    float temperature = 0;

    /*clear bits [1..0] (status bits)*/
    u16sT &= ~0x0003;
    /*calculate temperature [℃]*/
    /*T = -45 + 175 * rawValue / (2^16-1)*/
    temperature = (175 * (float)u16sT / 65535 - 45);

    return temperature;
}

/**
 * @brief 湿度计算
 *
 * @param u16sRH 读取到的湿度原始数据
 * @return float 计算后的湿度数据
 */
static float sht40_calc_RH(uint16_t u16sRH)
{
    float humidityRH = 0;

    /*clear bits [1..0] (status bits)*/
    u16sRH &= ~0x0003;
    /*calculate relative humidity [%RH]*/
    /*RH = rawValue / (2^16-1) * 10*/
    humidityRH = (100 * (float)u16sRH / 65535);

    return humidityRH;
}

/**
 * @brief sht40初始化
 *
 */
void sht40_init(void)
{
    uint32_t ret = 0;

    ret = IoTI2cInit(SHT40_I2C_PORT, EI2C_FRE_100K);
    if (ret != IOT_SUCCESS)
    {
        printf("i2c init fail!\r\v");
    }
}

/**
 * @brief 读取温度、湿度
 *
 * @param dat 数据指针
 */
void sht40_read_data(sht40_data_ptr dat)
{
    uint8_t send_data[] = {0xFD};
    IoTI2cWrite(SHT40_I2C_PORT, SHT40_I2C_ADDRESS, send_data, sizeof(send_data));

    delay_ms(10);

    uint8_t read_data[6] = {0};
    IoTI2cRead(SHT40_I2C_PORT, SHT40_I2C_ADDRESS, read_data, sizeof(read_data));

    if (sht40_check_crc(&read_data[0], 2, read_data[2])) {
        uint16_t u16_temperature = ((uint16_t)read_data[0] << 8) | read_data[1];
        dat->temperature         = sht40_calc_temperature(u16_temperature);
    }

    if (sht40_check_crc(&read_data[3], 2, read_data[5])) {
        uint16_t u16_humidity = ((uint16_t)read_data[3] << 8) | read_data[4];
        dat->humidity         = sht40_calc_RH(u16_humidity);
    }
}
