# RK2206实训箱操作手册—超声波测距传感器开发

HCSR04 模块（GPIO 接口）

## 目录

- [RK2206实训箱操作手册—超声波测距传感器开发](#rk2206实训箱操作手册超声波测距传感器开发)
  - [目录](#目录)
  - [实验内容](#实验内容)
  - [GPIO API说明](#gpio-api说明)
    - [函数说明](#函数说明)
      - [(一)、GPIO 初始化](#一gpio-初始化)
      - [(二)、GPIO 方向设置](#二gpio-方向设置)
      - [(三)、GPIO 电平读写](#三gpio-电平读写)
      - [(四)、GPIO 中断功能](#四gpio-中断功能)
    - [常量说明](#常量说明)
  - [HCSR04 超声波传感器 API说明](#hcsr04-超声波传感器-api说明)
    - [函数说明](#函数说明-1)
      - [(一)、HCSR04 初始化](#一hcsr04-初始化)
      - [(二)、HCSR04 反初始化](#二hcsr04-反初始化)
      - [(三)、HCSR04 获取距离数据](#三hcsr04-获取距离数据)
  - [代码编译](#代码编译)
    - [添加文件](#添加文件)
    - [修改 BUILD.gn 文件](#修改-buildgn-文件)
    - [编译结果](#编译结果)
    - [烧录固件](#烧录固件)
    - [运行结果](#运行结果)
  - [注意事项](#注意事项)
  - [技术规格](#技术规格)

---

## 实验内容

在RK2206实训箱上使用GPIO接口，实现超声波测距传感器（HCSR04）的编程开发。

---

## GPIO API说明

GPIO接口用于控制传感器的触发与回响信号，是本实验的底层接口。

---

### 函数说明

#### (一)、GPIO 初始化

| 定义：       | IoTGpioInit(unsigned int id)                        |
| ------------ | --------------------------------------------------- |
| **功能：**   | **初始化指定的GPIO设备**                             |
| **参数：**   | **`id`: GPIO编号**                                  |
| **返回值：** | **IOT_SUCCESS 成功 / IOT_FAILURE 失败**              |
| **依赖：**   | **`base/iot_hardware/peripheral/interfaces/kits/iot_gpio.h`** |
| **实现文件：** | **`hal_iot_gpio.c`（路径：device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite）** |

#### (二)、GPIO 方向设置

| 定义：       | IoTGpioSetDir(unsigned int id, IotGpioDir dir)      |
| ------------ | --------------------------------------------------- |
| **功能：**   | **设置GPIO引脚方向（输入/输出）**                    |
| **参数：**   | **`id`: GPIO编号；`dir`: 输入或输出方向**            |
| **返回值：** | **IOT_SUCCESS 成功 / IOT_FAILURE 失败**              |
| **取值：**   | **IOT_GPIO_DIR_IN（输入） / IOT_GPIO_DIR_OUT（输出）** |

#### (三)、GPIO 电平读写

| 函数 | 功能 | 定义 |
|------|------|------|
| `IoTGpioSetOutputVal(id, val)` | 设置输出电平 | 高/低电平控制 |
| `IoTGpioGetInputVal(id, *val)` | 读取输入电平 | 获取引脚状态 |
| **取值：** | **IOT_GPIO_VALUE0（低电平） / IOT_GPIO_VALUE1（高电平）** |  |

#### (四)、GPIO 中断功能

| 定义：       | IoTGpioRegisterIsrFunc(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity, GpioIsrCallbackFunc func, char *arg) |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **注册GPIO中断回调函数，用于响应信号变化**                   |
| **参数：**   | **GPIO编号 / 中断类型 / 极性 / 回调函数**                    |
| **返回值：** | **IOT_SUCCESS 成功 / IOT_FAILURE 失败**                      |
| **说明：**   | **可设置上升沿、下降沿或双沿触发，配合ECHO信号捕获时间差**   |

---

### 常量说明

| 名称 | 含义 |
| ---- | ---- |
| `IOT_GPIO_DIR_IN` | 输入方向 |
| `IOT_GPIO_DIR_OUT` | 输出方向 |
| `IOT_GPIO_VALUE0` | 低电平 |
| `IOT_GPIO_VALUE1` | 高电平 |

---

## HCSR04 超声波传感器 API说明

HCSR04 模块通过 GPIO 发送触发信号并测量回波时间，从而计算距离。

---

### 函数说明

#### (一)、HCSR04 初始化

| 定义：       | hcsr04_init(void)                                            |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **初始化 HCSR04 的 TRIG 与 ECHO 引脚**                       |
| **依赖：**   | **`hcsr04.h`**                                               |
| **说明：**   | **ECHO 配置为输入并注册中断；TRIG 配置为输出并拉低初始电平。** |

```c
void hcsr04_init(void)
{
    IoTGpioInit(ECHO_GPIO);
    IoTGpioSetDir(ECHO_GPIO, IOT_GPIO_DIR_IN);
    IoTGpioRegisterIsrFunc(ECHO_GPIO, IOT_INT_TYPE_EDGE, IOT_GPIO_EDGE_BOTH_TYPE, echo_gpio_isr_func, NULL);
    IoTGpioSetIsrMask(ECHO_GPIO, FALSE);

    IoTGpioInit(TRIG_GPIO);
    IoTGpioSetDir(TRIG_GPIO, IOT_GPIO_DIR_OUT);
    IoTGpioSetOutputVal(TRIG_GPIO, IOT_GPIO_VALUE0);
}
```

#### (二)、HCSR04 反初始化

| 定义：       | hcsr04_deinit(void)                                          |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **释放 GPIO 资源，关闭传感器接口**                           |
| **依赖：**   | **`hcsr04.h`**                                               |

```c
void hcsr04_deinit(void)
{
    IoTGpioDeinit(ECHO_GPIO);
    IoTGpioDeinit(TRIG_GPIO);
}
```

#### (三)、HCSR04 获取距离数据

| 定义：       | double hcsr04_get(void)                                      |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **测量距离（cm）**                                           |
| **返回值：** | **返回距离值（单位：厘米）**                                 |
| **说明：**   | **根据回波持续时间计算距离：距离 = 声速 × 时间 / 2**         |

```c
double hcsr04_get(void)
{
    if (echo_data.flag == ECHO_FLAG_CAPTURE_RISE) {
        hcsr04_send_trig();
    }

    uint64_t start_tick = LOS_TickCountGet();
    while (echo_data.flag != ECHO_FLAG_CAPTURE_SUCCESS) {
        uint64_t end_tick = LOS_TickCountGet();
        if (LOS_Tick2MS(end_tick - start_tick) > 200) {
            echo_data.flag = ECHO_FLAG_CAPTURE_RISE;
            return -1;
        }
    }
    echo_data.pulse_width = echo_data.time_fall - echo_data.time_rise;
    echo_data.flag = ECHO_FLAG_CAPTURE_RISE;

    double data = (double)echo_data.pulse_width / (double)ECHO_TIMER_FREQ * 170.0 * 100.0;
    return data;
}
```

---

## 代码编译

### 添加文件

将`~vendor/company_demo/b11_spi_rfid`这整个文件夹拖入`~/application/samples`路径下

![](/GPIO/b10_gpio_distance/images/2dad37eaae996cb3c7dc2826f6532412.png)   

### 修改 BUILD.gn 文件

修改 `vendor/lockzhiner/rk2206/sample` 路径下 BUILD.gn 文件，指定 `distance_example` 参与编译：

```r
"./b10_gpio_distance:distance_example",
```

修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下 Makefile 文件，添加：

```r
hardware_LIBS = -lhal_iothardware -lhardware -lshellcmd -ldistance_example
```


### 编译结果

![](/GPIO/b10_gpio_distance/images/image-20251021154701056.png)

### 烧录固件

打开烧录工具RKDevTool

![](/GPIO/b10_gpio_distance/images/image-20251021170153891.png)

指定两个烧录文件, 编译出的目标文件在 `out/rk2206/lockzhiner-rk2206/images`目录下。
![](/GPIO/b10_gpio_distance/images/image-20251021170850513.png)
在RK2206实训箱上上，长按左侧模块控制终端的MaskRom按键（MaskRom按键不要松开），点击ReSet按键（短按，按下后松开，但MaskRom按键不要松开，大约5秒后松开)，烧写工具出现：“发现一个MASKROM设备”。如下图所示：

![](/GPIO/b10_gpio_distance/images/image-20251021193107443.png)

此时点击“执行”，下载程序，下载完成如下：

![](/GPIO/b10_gpio_distance/images/image-20251021193256609.png)

### 运行结果

编译并烧录程序后，按下 RESET 按键，通过串口可看到如下输出：

```
entering kernel init...
hilog will init.
[MAIN:D]Main: LOS_Start ...
Entering scheduler
OHOS # hiview init success.
[GPIO:D]LzGpioInit: id 9 is initialized successfully
[GPIO:D]LzGpioInit: id 8 is initialized successfully
distance: 204.06
distance: 204.12
distance: 204.25
distance: 204.08
distance: 204.53
```

## 注意事项

1. **GPIO配置**：确保ECHO与TRIG分别为输入与输出模式。  
2. **供电要求**：HCSR04使用5V供电，信号线需与主控电平匹配。  
3. **时间延迟**：避免连续触发过快，建议触发周期≥60ms。  
4. **异常处理**：若返回-1，表示超时或未检测到回波信号。  

## 技术规格

- **通信接口**：GPIO  
- **工作电压**：5V  
- **检测范围**：2cm–400cm  
- **测量精度**：±3mm  
- **工作原理**：超声波发射与回波计时计算距离