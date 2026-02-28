# RK2206实训箱操作手册—ENS160开发

ENS160二氧化碳传感器

## 目录

- [RK2206实训箱操作手册—ENS160开发](#rk2206实训箱操作手册ens160开发)
  - [目录](#目录)
  - [实验内容](#实验内容)
  - [ENS160 API说明](#ens160-api说明)
    - [函数说明](#函数说明)
      - [(一)ENS160 初始化](#一ens160-初始化)
      - [(二)ENS160 设置温湿度补偿](#二ens160-设置温湿度补偿)
      - [(三)ENS160 读取 eCO2 数据](#三ens160-读取-eco2-数据)
    - [常量和枚举类型说明](#常量和枚举类型说明)
      - [I2C 端口](#i2c-端口)
      - [运行模式（ens160\_modes\_t）](#运行模式ens160_modes_t)
    - [寄存器地址定义](#寄存器地址定义)
  - [代码编译](#代码编译)
    - [添加文件](#添加文件)
    - [修改 BUILD.gn 文件](#修改-buildgn-文件)
    - [编译结果](#编译结果)
    - [烧录固件](#烧录固件)
    - [运行结果](#运行结果)
  - [注意事项](#注意事项)
  - [技术规格](#技术规格)

## 实验内容

在RK2206实训箱上使用I2C接口，实现ENS160二氧化碳传感器的编程开发。

## ENS160 API说明

I2C相关API详见PPT。

### 函数说明

#### (一)ENS160 初始化

| 定义：       | ens160_init(void)                                            |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **初始化 ENS160 传感器并进入工作模式**                       |
| **参数：**   | **void**                                                     |
| **返回值：** | **void**                                                     |
| **依赖：**   | **\\code_base\\b4_i2c_ens160\\include\\ens160.h**            |
| **Tips:**    | **初始化 I2C0_M2（400KHz），并设置设备运行模式为 Operational** |

#### (二)ENS160 设置温湿度补偿

| 定义：       | ens160_set_temperature_humidity(double temperature, double humidity) |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **为 ENS160 设置环境温度与湿度，用于补偿**                   |
| **参数：**   | **`temperature`: 温度(°C); `humidity`: 相对湿度(%RH)**       |
| **返回值：** | **void**                                                     |
| **依赖：**   | **\\code_base\\b4_i2c_ens160\\include\\ens160.h**            |
| **Tips:**    | **内部按数据手册转换为寄存器格式（温度 K×64、湿度 %×512）**  |

#### (三)ENS160 读取 eCO2 数据

| 定义：       | ens160_get_data(uint16_t *data)                             |
| ------------ | ----------------------------------------------------------- |
| **功能：**   | **读取 ENS160 的 eCO2 数据**                                |
| **参数：**   | **`data`: 指向存放 eCO2(ppm) 的指针**                       |
| **返回值：** | **void**                                                    |
| **依赖：**   | **\\code_base\\b4_i2c_ens160\\include\\ens160.h**           |
| **Tips:**    | **函数内部先写寄存器地址后再读 2 字节数据并拼接为 16 位值** |

### 常量和枚举类型说明

#### I2C 端口

- `EI2C0_M2`

#### 运行模式（ens160_modes_t）

- `DEEP_SLEEP_MODE` (0x00)
- `IDLE_MODE`
- `OPERATIONAL_MODE`

### 寄存器地址定义

| 名称            | 地址 | 功能描述                                    |
| --------------- | ---- | ------------------------------------------- |
| `PART_ID`       | 0x00 | 器件 ID/芯片识别号                          |
| `OPMODE`        | 0x10 | 运行模式选择（Deep Sleep/Idle/Operational） |
| `CONFIG`        | 0x11 | 配置寄存器（功能与行为配置）                |
| `COMMAND`       | 0x12 | 命令寄存器（执行特定操作/复位/触发等）      |
| `TEMP_IN`       | 0x13 | 温度补偿输入（单位：K×64 的无符号整数）     |
| `RH_IN`         | 0x15 | 湿度补偿输入（单位：%×512 的无符号整数）    |
| `DEVICE_STATUS` | 0x20 | 设备状态（错误/就绪/新数据标志等）          |
| `DATA_AQI`      | 0x21 | 空气质量指数 AQI（分级/指数值）             |
| `DATA_TVOC`     | 0x22 | 总挥发性有机物 TVOC 数据                    |
| `DATA_ECO2`     | 0x24 | 等效二氧化碳浓度 eCO2 数据（ppm）           |
| `DATA_T`        | 0x30 | 传感器内部温度读数（格式依器件定义）        |
| `DATA_RH`       | 0x32 | 传感器内部相对湿度读数（格式依器件定义）    |
| `DATAMISR`      | 0x38 | 数据一致性/校验相关寄存器（用于数据有效性） |
| `GPR_WRITE`     | 0x40 | 通用目的寄存器写入口（固件/算法交互）       |
| `GPR_READ`      | 0x48 | 通用目的寄存器读出口（固件/算法交互）       |

## 代码编译

### 添加文件

将`~vendor/company_demo/b4_i2c_ens160`这整个文件夹拖入`~/application/samples`路径下

![](/I2C/b4_i2c_ens160/images/image-20251021155046128.png)

### 修改 BUILD.gn 文件

修改 `~/application/samples/` 路径下 BUILD.gn 文件，将如下代码添加到对应位置，指定 `ens160_example` 参与编译。

```r
"./b4_i2c_ens160:ens160_example",
```

![](/I2C/b4_i2c_ens160/images/image-20251021155448421.png)

修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下 Makefile 文件，添加 `-lens160_example` 参与编译。

```r
hardware_LIBS = -lhal_iothardware -lhardware -lshellcmd -lens160_example
```

![](/I2C/b4_i2c_ens160/images/image-20251021155332850.png)

### 编译结果

![](/I2C/b4_i2c_ens160/images/image-20251021154701056.png)

### 烧录固件

打开烧录工具RKDevTool

![](/I2C/b4_i2c_ens160/images/image-20251021170153891.png)

指定两个烧录文件, 编译出的目标文件在 `out/rk2206/lockzhiner-rk2206/images`目录下。
![](/I2C/b4_i2c_ens160/images/image-20251021170850513.png)
在RK2206实训箱上上，长按左侧模块控制终端的MaskRom按键(MaskRom按键不要松开），点击ReSet按键（短按，按下后松开，但MaskRom按键不要松开，大约5秒后松开)，烧写工具出现：“发现一个MASKROM设备”。如下图所示：

![](/I2C/b4_i2c_ens160/images/image-20251021193107443.png)

此时点击“执行”，下载程序，下载完成如下：

![](/I2C/b4_i2c_ens160/images/image-20251021200134360.png)

### 运行结果

示例代码编译烧录代码后，按下开发板的RESET按键，连接串口，可以看到二氧化碳浓度信息。

```
entering kernel init...
hilog will init.
[MAIN:D]Main: LOS_Start ...
Entering scheduler
OHOS # hiview init success.
[GPIO:D]LzGpioInit: id 1 is initialized successfully
[GPIO:D]LzGpioInit: id 0 is initialized successfully
co2: 400
co2: 400
co2: 400
co2: 400
co2: 400
co2: 400
```

<img src="https://github.com/WiBeeBaBu/rk2206_practice_kit/blob/master/Src/b4_i2c_ens160/images/image-20251021200134360.png" width="100%" style="max-width: 800px;" />

## 注意事项

1. **初始化顺序**: 必须先调用 `ens160_init()` 再进行补偿/读取
2. **I2C 通信**: 设备地址 `0x52`，总线频率 400KHz
3. **补偿建议**: 提供实时温湿度能改善测量准确度
4. **数据单位**: `ens160_get_data` 返回 eCO2，单位 ppm

## 技术规格

- **通信接口**: I2C
- **I2C 地址**: 0x52
- **输出**: eCO2、TVOC（本驱动示例读取 eCO2）
- **运行模式**: Deep Sleep / Idle / Operational