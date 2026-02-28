#pragma once

#include <stdbool.h>
#include <stdint.h>

#define VEML7700_ALS_CONF_0   0x00 /* ALS gain, integration time, interrupt, and shutdown */
#define VEML7700_ALS_WH       0x01 /* ALS high threshold window setting */
#define VEML7700_ALS_WL       0x02 /* ALS low threshold window setting */
#define VEML7700_POWER_SAVING 0x03 /* Power saving */
#define VEML7700_ALS          0x04 /* data of whole ALS 16 bits */
#define VEML7700_WHITE        0x05 /* data of whole WHITE 16 bits */
#define VEML7700_ALS_INT      0x06 /* ALS INT trigger event */

/* Gain selection */
typedef enum ALS_GAIN {
    ALS_GAIN_1 = 0, /* 00 = ALS gain x 1 */
    ALS_GAIN_2,     /* 01 = ALS gain x 2 */
    ALS_GAIN_3,     /* 10 = ALS gain x (1/8) */
    ALS_GAIN_4,     /* 11 = ALS gain x (1/4) */
} als_gain_t;

/* ALS integration time setting */
typedef enum ALS_IT {
    ALS_IT_25MS  = 0b00001100, /* 25 ms */
    ALS_IT_50MS  = 0b00001000, /* 50 ms */
    ALS_IT_100MS = 0b00000000, /* 100 ms */
    ALS_IT_200MS = 0b00000001, /* 200 ms */
    ALS_IT_400MS = 0b00000010, /* 400 ms */
    ALS_IT_800MS = 0b00000011, /* 800 ms */
} als_it_t;

/* ALS persistence protect number setting */
typedef enum ALS_PERS {
    ALS_PERS_1 = 0, /* 1 */
    ALS_PERS_2,     /* 2 */
    ALS_PERS_4,     /* 4 */
    ALS_PERS_8,     /* 8 */
} als_pers_t;

/* ALS interrupt enable setting */
typedef enum ALS_INT_EN {
    ALS_INT_DISABLE = 0, /* ALS INT disable */
    ALS_INT_ENABLE,      /* ALS INT enable */
} als_int_en_t;

/* ALS shut down setting */
typedef enum ALS_SD {
    ALS_SD_POWERON = 0, /* ALS power on */
    ALS_SD_SHUTDOWN,    /* ALS shutdown */
} als_sd_t;

/*  ALS gain, integration time, interrupt, and shutdown */
typedef union ALS_CONF_0 {
    uint16_t value;
    struct {
        uint16_t ALS_SD : 1;
        uint16_t ALS_INT_EN : 1;
        uint16_t Reserved_2bit : 2;
        uint16_t ALS_PERS : 2;
        uint16_t ALS_IT : 4;
        uint16_t Reserved_1bit : 1;
        uint16_t ALS_GAIN : 2;
        uint16_t Reserved_3bit : 3;
    };
} als_conf_0_t, *als_conf_0_ptr;

/* Power saving mode; see table “Refresh time” */
typedef enum PSM_MODE {
    PSM_MODE_1 = 0, /* mode 1 */
    PSM_MODE_2,     /* mode 2 */
    PSM_MODE_3,     /* mode 3 */
    PSM_MODE_4,     /* mode 4 */
} psm_mode_t;

/* Power saving mode enable setting */
typedef enum PSM_EN {
    PSM_DISABLE = 0, /* PSM disable */
    PSM_ENABLE,      /* PSM enable */
} psm_en_t;

/* Power Saving Mode */
typedef union PSM {
    uint16_t value;
    struct {
        uint16_t PSM_EN : 1;
        uint16_t PSM_MODE : 2;
        uint16_t Reserved : 13;
    };
} psm_t, *psm_ptr;

void veml7700_init(void);
void veml7700_set_als_conf_0(als_gain_t gain, als_it_t it, als_pers_t pers, als_int_en_t int_en, als_sd_t sd);
void veml7700_set_psm(psm_mode_t psm_mode, psm_en_t psm_en);
void veml7700_read_data(double *dat);
