# RK2206_Practice_Kit

RK2206 实训箱基础示例与操作手册：提供 **文档（Docs）** 与 **源码（Src）**，便于在 OpenHarmony 环境下进行外设与通信接口开发。

---

## 项目结构

```
RK2206_Practice_Kit/         # 示例源码（可编译、可烧录、包含pdf文件）
├── ADC/                   
│   ├── b6_adc_mq2           # MQ2 烟雾
├── GPIO
|   ├── b7_gpio_fire         # 火焰传感器
|   ├── b8_gpio_body         # 人体感应传感器
|   ├── b10_gpio_distance    # HCSR04 超声波
├── I2C/
|   ├── b4_i2c_ens160        # ENS160 二氧化碳
|   ├── b5_i2c_sht40         # SHT40 温湿度
|   ├── b9_i2c_veml7700      # VEML7700 光照
├── PWM/
|   ├── b1_pwm_sg90          # SG90 舵机
|   ├── b3_pwm_beep          # 蜂鸣器
├── SPI/
|   ├── b11_spi_rfid         # MFRC522 RFID
├── UART/
|   ├── b12_iot_uart         # UART 串口通信
├── RK2206_Practice_Kit.md
├── RK2206实训箱-编译、烧录、测试.pdf
└── RK2206实训箱开发.pdf
```
---

## 示例总览

| 编号 | 类型 | 目录（源码） | 文档（PDF） | 功能简介 |
| :---: | :---: | :--- | :--- | :--- |
| 1 | ADC | [b6_adc_mq2](/ADC/b6_adc_mq2) | [ADC-烟雾传感器](/ADC/b6_adc_mq2/ADC-烟雾传感器.pdf) | 通过 ADC 读取 MQ2 烟雾浓度(PPM)，支持校准与反初始化。 |
| 2 | GPIO | [b8_gpio_body](/GPIO/b8_gpio_body) | [GPIO-人体感应传感器](/GPIO/b8_gpio_body/GPIO-人体感应传感器.pdf) | 通过 GPIO 检测人体运动(HC-SR501)，串口打印检测状态。 |
| 3 | GPIO | [b7_gpio_fire](/GPIO/b7_gpio_fire) | [GPIO-火焰传感器](/GPIO/b7_gpio_fire/GPIO-火焰传感器.pdf) | 通过 GPIO 检测火焰，低电平表示检测到火焰。 |
| 4 | GPIO | [b10_gpio_distance](/GPIO/b10_gpio_distance) | [GPIO-超声波传感](/GPIO/b10_gpio_distance/GPIO-超声波传感.pdf) | 通过 GPIO 控制 HCSR04 超声波模块，实现测距。 |
| 5 | GPIO | [b2_gpio_rgb](/GPIO/b2_gpio_rgb) |[GPIO-RGB](/GPIO/b2_gpio_rgb/GPIO-RGB.pdf)| 通过 GPIO 驱动 RGB 灯，实现颜色与亮灭控制。 |
| 6 | I2C | [b4_i2c_ens160](/I2C/b4_i2c_ens160) | [I2C-二氧化碳传感器](/I2C/b4_i2c_ens160/I2C-二氧化碳传感器.pdf) | 通过 I2C 读取 ENS160 二氧化碳(eCO2)，支持温湿度补偿。 |
| 7 | I2C | [b9_i2c_veml7700](/I2C/b9_i2c_veml7700) | [I2C-光照传感器](/I2C/b9_i2c_veml7700/I2C-光照传感器.pdf) | 通过 I2C 读取 VEML7700 环境光强度，可配置增益与省电模式。 |
| 8 | I2C | [b5_i2c_sht40](/I2C/b5_i2c_sht40) | [I2C-温湿度传感器](/I2C/b5_i2c_sht40/I2C-温湿度传感器.pdf) | 通过 I2C 读取 SHT40 温湿度数据，含 CRC 校验。 |
| 9 | PWM | [b1_pwm_sg90](/PWM/b1_pwm_sg90) | [PWM-SG90舵机](/PWM/b1_pwm_sg90/PWM-SG90舵机.pdf) | 以 PWM 驱动 SG90 舵机，实现 0°～180° 角度控制。 |
| 10 | PWM | [b3_pwm_beep](/PWM/b3_pwm_beep) | [PWM-蜂鸣器](/PWM/b3_pwm_beep/PWM-蜂鸣器.pdf) | 以 PWM 驱动蜂鸣器，实现开关控制(500Hz，50% 占空比)。 |
| 11 | SPI | [b11_spi_rfid](/SPI/b11_spi_rfid) | [SPI-射频模块](/SPI/b11_spi_rfid/SPI-射频模块.pdf) | 通过 SPI 与 MFRC522 通信，实现 RFID 寻卡与卡号(UID)读取。 |
| 12 | UART | [b12_iot_uart](/UART/b12_iot_uart) | [UART-串口通信](/UART/b12_iot_uart/UART-串口通信.pdf) | 通过 UART 实现串口收发，并在终端查看读写结果。 |
