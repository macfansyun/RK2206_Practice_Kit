#include "ohos_init.h"
#include "los_task.h"

#include "rc522.h"

#include <stdbool.h>
#include <stdio.h>

/* 线程id */
static uint32_t m_thread_id;

/**
 * @brief oled 线程
 *
 * @param args
 */
static void spi_rfid_thread(void *args)
{
    uint8_t rfid[18]      = {0};
    uint8_t last_rfid[18] = {0};

    mfrc522_init(); // rfid 初始化

    while (1) {
        if (!mfrc522_request(PICC_REQIDL, rfid)) {
            /* 获取 rfid */
            if (!mfrc522_anticoll(rfid)) {
                /* 判断是否是新卡 */
                bool is_update = false;
                for (uint8_t i = 0; i < 4; i++) {
                    if (rfid[i] != last_rfid[i]) {
                        is_update = true;
                        break;
                    }
                }

                /* 新卡 */
                if (is_update) {
                    for (uint8_t i = 0; i < 4; i++) {
                        last_rfid[i] = rfid[i];
                    }
                    uint8_t rfid_str[18] = {0};
                    snprintf(rfid_str, sizeof(rfid_str), "%02X%02X%02X%02X", rfid[0], rfid[1], rfid[2], rfid[3]);
                    printf("New RFID: %s\r\n", rfid_str);
                }
            }
        }
        LOS_Msleep(1000);
    }
}

/**
 * @brief rfid spi 使用例程
 *
 */
void rfid_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)spi_rfid_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "spi_rfid_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}

APP_FEATURE_INIT(rfid_example);
