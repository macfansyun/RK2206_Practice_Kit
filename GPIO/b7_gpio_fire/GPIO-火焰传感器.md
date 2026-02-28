# RK2206实训箱操作手册—火焰传感器开发

火焰传感器

## 目录

- [RK2206实训箱操作手册—火焰传感器开发](#rk2206实训箱操作手册火焰传感器开发)
  - [目录](#目录)
  - [实验内容](#实验内容)
  - [Fire API说明](#fire-api说明)
    - [函数说明](#函数说明)
      - [(一)、火焰传感器 初始化](#一火焰传感器-初始化)
      - [(二)、火焰传感器 反初始化](#二火焰传感器-反初始化)
      - [(三)、火焰传感器 读取火焰状态](#三火焰传感器-读取火焰状态)
    - [常量说明](#常量说明)
      - [GPIO 引脚](#gpio-引脚)
  - [代码编译](#代码编译)
    - [添加文件](#添加文件)
    - [修改 BUILD.gn 文件](#修改-buildgn-文件)
    - [编译结果](#编译结果)
  - [烧录固件](#烧录固件)
  - [运行结果](#运行结果)
  - [注意事项](#注意事项)
  - [技术规格](#技术规格)

## 实验内容

在RK2206实训箱上使用GPIO接口，实现火焰传感器的编程开发，通过数字输入检测是否有火焰出现，并在串口打印火焰状态。

## Fire API说明

GPIO相关API详见PPT（如 `IoTGpioInit`、`IoTGpioDeinit`、`IoTGpioGetInputVal` 等），本节重点说明火焰传感器封装接口。

### 函数说明

#### (一)、火焰传感器 初始化

| 定义：       | `void fire_init(void)`                           |
| ------------ | ----------------------------------------------- |
| **功能：**   | **初始化火焰传感器模块，配置对应GPIO引脚**     |
| **参数：**   | **void**                                        |
| **返回值：** | **void**                                        |
| **依赖：**   | **`\\code_base\\b7_gpio_fire\\include\\fire.h`** |
| **Tips:**    | **内部会调用 `IoTGpioInit(FIRE_GPIO)` 完成GPIO初始化** |

#### (二)、火焰传感器 反初始化

| 定义：       | `void fire_deinit(void)`                         |
| ------------ | ----------------------------------------------- |
| **功能：**   | **反初始化火焰传感器模块，释放GPIO资源**       |
| **参数：**   | **void**                                        |
| **返回值：** | **void**                                        |
| **依赖：**   | **`\\code_base\\b7_gpio_fire\\include\\fire.h`** |
| **Tips:**    | **在不再使用火焰传感器时调用，避免占用多余GPIO资源** |

#### (三)、火焰传感器 读取火焰状态

| 定义：       | `bool get_fire_value(void)`                      |
| ------------ | ----------------------------------------------- |
| **功能：**   | **读取火焰传感器当前检测状态**                  |
| **参数：**   | **void**                                        |
| **返回值：** | **`bool`：火焰检测结果**                         |
| **依赖：**   | **`\\code_base\\b7_gpio_fire\\include\\fire.h`** |
| **Tips:**    | **内部通过 `IoTGpioGetInputVal(FIRE_GPIO, &gpio_value)` 获取输入电平，当输入为低电平(`IOT_GPIO_VALUE0`)时返回 `true`，表示检测到火焰** |

返回值说明：

| 返回值 | 描述         |
| ------ | ------------ |
| `true` | 检测到火焰   |
| `false`| 未检测到火焰 |

### 常量说明

#### GPIO 引脚

- `FIRE_GPIO`: `GPIO0_PB4`
  - 火焰传感器数字输出信号连接到 `GPIO0_PB4` 引脚
  - 该引脚在内部被用作数字输入，读取传感器高/低电平

## 代码编译

### 添加文件

将 `~vendor/company_demo/b7_gpio_fire` 这整个文件夹拖入 `~/application/samples` 路径下。

![](/GPIO/b7_gpio_fire/images/image-20251120145258612.png)

### 修改 BUILD.gn 文件

修改 `~/application/samples/` 路径下 BUILD.gn 文件，将如下代码添加到对应位置，指定 `fire_example` 参与编译。

```r
"./b7_gpio_fire:fire_example",
```

修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下 Makefile 文件，添加 `-lfire_example` 参与编译。

```r
hardware_LIBS = -lhal_iothardware -lhardware -lshellcmd -lfire_example
```

### 编译结果

![](/GPIO/b7_gpio_fire/images/62944df3881d43da0a226ebf933d43f0.png)

## 烧录固件

打开烧录工具RKDevTool。
![](/GPIO/b7_gpio_fire/images/image-20251021170153891.png)
指定两个烧录文件，编译出的目标文件在 `out/rk2206/lockzhiner-rk2206/images` 目录下。
![](/GPIO/b7_gpio_fire/images/image-20251021170850513.png)
在RK2206实训箱上，长按左侧模块控制终端的 MaskRom 按键（MaskRom 按键不要松开），点击 Reset 按键（短按，按下后松开，但 MaskRom 按键不要松开，大约5秒后松开)。
![](/GPIO/b7_gpio_fire/images/image-20251021193107443.png)
烧写工具出现："发现一个MASKROM设备" 提示后，点击“执行”，开始下载程序。
![](/GPIO/b7_gpio_fire/images/image-20251021193256609.png)


## 运行结果

示例代码编译烧录代码后，按下开发板的 RESET 按键，连接串口，可以看到打印火焰信息。使用打火机模拟场景，可以看到检测到火焰时，串口打印 `true`。

```text
hilog will init.
[MAIN:D]Main: LOS_Start ...
Entering scheduler
OHOS # hiview init success.
[GPIO:D]LzGpioInit: id 12 is initialized successfully
fire: false
fire: false
fire: false
fire: false
fire: true
fire: true
fire: true
fire: false
fire: false
fire: false
```

![](/GPIO/b7_gpio_fire/images/586c2f854fb1098868c8ddf4e17f8812.png)

## 注意事项

1. **GPIO方向与电平**：火焰传感器为数字量输出模块，`FIRE_GPIO` 配置为输入模式，由模块内部比较器输出高/低电平，程序通过 `IoTGpioGetInputVal` 读取该引脚电平，并根据电平判断是否有火焰。
2. **信号极性**：当前驱动中，当输入电平为低(`IOT_GPIO_VALUE0`)时返回 `true`，表示检测到火焰；若要更改极性，可在 `get_fire_value()` 内部反转判断逻辑。
3. **供电与接线**：确保火焰传感器模块按实训箱接线图正确连接到RK2206开发板，供电电压符合模块规格，禁止带电随意插拔模块。
4. **火源安全**：实验时使用打火机或明火请注意实验环境安全，避免靠近易燃物，请在通风良好、无易燃易爆物的环境中操作。
5. **多次初始化**：`fire_init()` 一般在任务创建前或任务内调用一次即可，不建议频繁重复初始化和反初始化。

## 技术规格

- **通信接口**: GPIO（数字输入）
- **GPIO引脚**: `GPIO0_PB4`（数字信号输入）
- **输出类型**: 数字高/低电平，表示“有火焰/无火焰”
- **工作电压**: 依传感器模块规格，一般为 3.3V 或 5V（以实训箱配套模块为准）
- **检测对象**: 火焰/可见光源（对一定波段的红外/可见光敏感）
- **响应时间**: 毫秒级数字响应（受模块硬件电路影响）
- **典型应用**: 火焰检测、打火机火焰演示、小型火灾预警等场景
