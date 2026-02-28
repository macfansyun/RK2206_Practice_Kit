# RK2206实训箱操作手册—SHT40开发

SHT40温湿度传感器

## 目录

- [RK2206实训箱操作手册—SHT40开发](#rk2206实训箱操作手册sht40开发)
  - [目录](#目录)
  - [实验内容](#实验内容)
  - [SHT40 API说明](#sht40-api说明)
    - [函数说明](#函数说明)
      - [(一)SHT40 初始化](#一sht40-初始化)
      - [(二)SHT40 读取温湿度](#二sht40-读取温湿度)
    - [常量说明](#常量说明)
      - [I2C 端口](#i2c-端口)
      - [**I2C 地址**:](#i2c-地址)
  - [代码编译](#代码编译)
    - [添加文件](#添加文件)
    - [修改 BUILD.gn 文件](#修改-buildgn-文件)
    - [编译结果](#编译结果)
    - [烧录固件](#烧录固件)
    - [运行结果](#运行结果)
  - [注意事项](#注意事项)
  - [技术规格](#技术规格)

## 实验内容

在RK2206实训箱上使用I2C接口，实现SHT40温湿度传感器的编程开发。

## SHT40 API说明

I2C相关API详见PPT。

### 函数说明

#### (一)SHT40 初始化

| 定义：       | sht40_init(void)                                             |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **初始化 SHT40 温湿度传感器模块**                            |
| **参数：**   | **void**                                                     |
| **返回值：** | **void**                                                     |
| **依赖：**   | **\\code_base\\b5_i2c_sht40\\include\\sht40.h**              |
| **Tips:**    | **该接口只需调用一次，不支持重复调用。内部会初始化 I2C0_M2 端口，频率为 100KHz** |

#### (二)SHT40 读取温湿度

| 定义：       | sht40_read_data(sht40_data_ptr dat)                          |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **读取 SHT40 的温度与相对湿度**                              |
| **参数：**   | **`dat`: 指向存储温度 (°C) 与湿度 (%RH) 的结构体指针**       |
| **返回值：** | **void**                                                     |
| **依赖：**   | **\\code_base\\b5_i2c_sht40\\include\\sht40.h**              |
| **Tips:**    | **内部包含 CRC 校验；温度、湿度会按数据手册公式转换为物理量** |

### 常量说明

#### I2C 端口

- `EI2C0_M2`

#### **I2C 地址**:

* `0x44`

## 代码编译

### 添加文件

将`~vendor/company_demo/b5_i2c_sht40`这整个文件夹拖入`~/application/samples`路径下

![](/I2C/b5_i2c_sht40/images/image-20251021160949682.png)

### 修改 BUILD.gn 文件

修改 `~/application/samples/` 路径下 BUILD.gn 文件，将如下代码添加到对应位置，指定 `sht40_example` 参与编译。

```r
"./b5_i2c_sht40:sht40_example",
```

![](/I2C/b5_i2c_sht40/images/image-20251021161455324.png)

修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下 Makefile 文件，添加 `-lveml7700_example` 参与编译。

```r
hardware_LIBS = -lhal_iothardware -lhardware -lshellcmd -lsht40_example
```

![](/I2C/b5_i2c_sht40/images/image-20251021161348725.png)

### 编译结果

![](/I2C/b5_i2c_sht40/images/image-20251021162125890.png)

### 烧录固件

打开烧录工具RKDevTool

![](/I2C/b5_i2c_sht40/images/image-20251021170153891.png)

指定两个烧录文件, 编译出的目标文件在 `out/rk2206/lockzhiner-rk2206/images`目录下。
![](/I2C/b5_i2c_sht40/images/image-20251021170850513.png)
在RK2206实训箱上上，长按左侧模块控制终端的MaskRom按键（MaskRom按键不要松开），点击ReSet按键（短按，按下后松开，但MaskRom按键不要松开，大约5秒后松开)，烧写工具出现：“发现一个MASKROM设备”。如下图所示：

![](/I2C/b5_i2c_sht40/images/image-20251021193107443.png)

此时点击“执行”，下载程序，下载完成如下：

![](/I2C/b5_i2c_sht40/images/image-20251021193256609.png)

### 运行结果

示例代码编译烧录代码后，按下开发板的RESET按键，连接串口，可以看到温湿度信息。

```
entering kernel init...
hilog will init.
[MAIN:D]Main: LOS_Start ...
Entering scheduler
OHOS # hiview init success.
[GPIO:D]LzGpioInit: id 1 is initialized successfully
[GPIO:D]LzGpioInit: id 0 is initialized successfully
temperature: 28.43, humidity: 41.67
temperature: 28.44, humidity: 41.66
temperature: 28.46, humidity: 41.68
temperature: 28.47, humidity: 41.67
temperature: 28.44, humidity: 41.69
temperature: 28.48, humidity: 41.66
temperature: 28.46, humidity: 41.66
temperature: 28.46, humidity: 41.67
temperature: 28.44, humidity: 41.68
temperature: 28.43, humidity: 41.69
temperature: 28.41, humidity: 41.65
temperature: 28.46, humidity: 41.64
temperature: 28.44, humidity: 41.63
temperature: 28.43, humidity: 41.61
temperature: 28.44, humidity: 41.64
temperature: 28.43, humidity: 41.59
temperature: 28.43, humidity: 41.63
temperature: 28.43, humidity: 41.64
temperature: 28.42, humidity: 41.62
temperature: 28.44, humidity: 41.64
```

![](/I2C/b5_i2c_sht40/images/image-20251021195903291.png)

## 注意事项

1. **初始化顺序**: 必须先调用 `sht40_init()` 再进行读取
2. **I2C 通信**: 设备地址为 `0x44`，命令触发后需等待约 10ms 再读数据
3. **CRC 校验**: 驱动已自动进行 CRC 校验，校验失败会跳过对应物理量更新
4. **数据单位**: 温度单位为 °C，湿度单位为 %RH

## 技术规格

- **通信接口**: I2C
- **I2C 地址**: 0x44
- **输出**: 温度与相对湿度
- **分辨率**: 16 位原始数据（内部转换）
- **典型测量等待**: 10 ms