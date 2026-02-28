#include "veml7700.h"

#include "iot_i2c.h"
#include "iot_errno.h"

/* veml7700对应i2c */
#define VEML7700_I2C_PORT EI2C0_M2

/* veml7700地址 */
#define VEML7700_I2C_ADDRESS 0x10

/**
 * @brief vwml7700 初始化
 *
 */
void veml7700_init(void)
{
    uint32_t ret      = 0;

    ret = IoTI2cInit(VEML7700_I2C_PORT, EI2C_FRE_100K);
    if (ret != IOT_SUCCESS) {
        printf("i2c init fail!\r\v");
    }
}

/**
 * @brief veml7700 读取数据
 *
 * @param dat 数据
 */
void veml7700_read_data(double *dat)
{
    uint8_t send_data[]  = {VEML7700_ALS};
    uint8_t read_data[2] = {0};
    IoTI2cWriteRead(VEML7700_I2C_PORT, VEML7700_I2C_ADDRESS, send_data, sizeof(send_data), read_data, sizeof(read_data));

    uint16_t u16_data = (read_data[1] << 8) | read_data[0];
    *dat              = (double)u16_data * 2.1504;
}

/**
 * @brief veml7700 设置 als_conf_0
 *
 * @param gain Gain
 * @param it ALS integration time setting
 * @param pers ALS persistence protect number setting
 * @param int_en ALS interrupt enable setting
 * @param sd ALS shut down setting
 */
void veml7700_set_als_conf_0(als_gain_t gain, als_it_t it, als_pers_t pers, als_int_en_t int_en, als_sd_t sd)
{
    uint32_t ret      = 0;
    als_conf_0_t conf = {0};

    conf.ALS_GAIN   = gain;
    conf.ALS_IT     = it;
    conf.ALS_PERS   = pers;
    conf.ALS_INT_EN = int_en;
    conf.ALS_SD     = sd;

    uint8_t send_data[] = {VEML7700_ALS_CONF_0, conf.value & 0xFF, conf.value >> 8};
    ret                 = IoTI2cWrite(VEML7700_I2C_PORT, VEML7700_I2C_ADDRESS, send_data, sizeof(send_data));
    if (ret != IOT_SUCCESS) {
        printf("i2c write fail!\r\v");
    }
}

/**
 * @brief veml7700 设置 psm
 *
 * @param psm_mode Power saving mode; see table “Refresh time”
 * @param psm_en Power saving mode enable setting
 */
void veml7700_set_psm(psm_mode_t psm_mode, psm_en_t psm_en)
{
    uint32_t ret = 0;
    psm_t psm    = {0};

    psm.PSM_MODE = psm_mode;
    psm.PSM_EN   = psm_en;

    uint8_t send_data[] = {VEML7700_POWER_SAVING, psm.value & 0xFF, psm.value >> 8};
    ret                 = IoTI2cWrite(VEML7700_I2C_PORT, VEML7700_I2C_ADDRESS, send_data, sizeof(send_data));
    if (ret != IOT_SUCCESS) {
        printf("i2c write fail!\r\v");
    }
}
