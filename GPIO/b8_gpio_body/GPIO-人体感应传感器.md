# RK2206实训箱操作手册—人体感应传感器开发

人体感应传感器（HC-SR501）

## 目录

- [RK2206实训箱操作手册—人体感应传感器开发](#rk2206实训箱操作手册人体感应传感器开发)
  - [目录](#目录)
  - [实验内容](#实验内容)
  - [人体感应 API说明](#人体感应-api说明)
    - [函数说明](#函数说明)
      - [(一)、人体感应 初始化](#一人体感应-初始化)
      - [(二)、人体感应 反初始化](#二人体感应-反初始化)
      - [(三)、人体感应 读取状态](#三人体感应-读取状态)
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

在RK2206实训箱上使用GPIO接口，实现人体感应（PIR）传感器的编程开发。通过数字输入检测是否有人体运动触发，并在串口打印当前检测状态。

## 人体感应 API说明

GPIO相关API详见PPT（如 `IoTGpioInit`、`IoTGpioSetDir`、`IoTGpioGetInputVal` 等），本节重点说明人体感应传感器封装接口。

### 函数说明

#### (一)、人体感应 初始化

| 定义：       | `void hcsr501_init(void)`                           |
| ------------ | -------------------------------------------------- |
| **功能：**   | **初始化人体感应传感器模块，配置对应GPIO为输入** |
| **参数：**   | **void**                                           |
| **返回值：** | **void**                                           |
| **依赖：**   | **`\\code_base\\b8_gpio_body\\include\\hcsr501.h`** |
| **Tips:**    | **内部会调用 `IoTGpioInit(HCSR501_GPIO)` 并设置方向为输入** |

#### (二)、人体感应 反初始化

| 定义：       | `void hcsr501_deinit(void)`                         |
| ------------ | -------------------------------------------------- |
| **功能：**   | **反初始化人体感应传感器模块，释放GPIO资源**     |
| **参数：**   | **void**                                           |
| **返回值：** | **void**                                           |
| **依赖：**   | **`\\code_base\\b8_gpio_body\\include\\hcsr501.h`** |
| **Tips:**    | **在不再使用人体感应传感器时调用，避免占用GPIO** |

#### (三)、人体感应 读取状态

| 定义：       | `bool hcsr501_get(void)`                            |
| ------------ | -------------------------------------------------- |
| **功能：**   | **读取人体感应传感器当前触发状态**                |
| **参数：**   | **void**                                           |
| **返回值：** | **`bool`：人体检测结果**                            |
| **依赖：**   | **`\\code_base\\b8_gpio_body\\include\\hcsr501.h`** |
| **Tips:**    | **内部通过 `IoTGpioGetInputVal(HCSR501_GPIO, &value)` 读取电平，高电平(`IOT_GPIO_VALUE1`)表示检测到人体活动** |

返回值说明：

| 返回值 | 描述         |
| ------ | ------------ |
| `true` | 检测到人体   |
| `false`| 未检测到人体 |

### 常量说明

#### GPIO 引脚

- `HCSR501_GPIO`: `GPIO0_PB5`
  - 人体感应传感器数字输出信号连接到 `GPIO0_PB5` 引脚
  - 驱动中会将其配置为输入模式，实时读取高/低电平

## 代码编译

### 添加文件

将 `~vendor/company_demo/b8_gpio_body` 整个文件夹拖入 `~/application/samples` 路径下。

![](/GPIO/b8_gpio_body/images/image-20251120145753316.png)

### 修改 BUILD.gn 文件

修改 `~/application/samples/` 路径下 BUILD.gn 文件，添加如下条目以编译 `body_example`：

```r
"./b8_gpio_body:body_example",
```

修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下 Makefile 文件，添加 `-lbody_example`：

```r
hardware_LIBS = -lhal_iothardware -lhardware -lshellcmd -lbody_example
```

### 编译结果

![](/GPIO/b8_gpio_body/images/62944df3881d43da0a226ebf933d43f0.png)

## 烧录固件

打开烧录工具 RKDevTool。
![](/GPIO/b8_gpio_body/images/image-20251021170153891.png)
指定两个烧录文件，编译输出位于 `out/rk2206/lockzhiner-rk2206/images`。
![](/GPIO/b8_gpio_body/images/image-20251021170850513.png)
在实训箱上长按 MaskRom 按键，短按 Reset（松开后继续按住 MaskRom 约5秒），等待工具提示“发现一个MASKROM设备”。
![](/GPIO/b8_gpio_body/images/image-20251021193107443.png)
点击“执行”开始下载程序，完成后提示烧录成功。
![](/GPIO/b8_gpio_body/images/image-20251021193256609.png)

## 运行结果

示例编译并烧录后，按下开发板 RESET，串口可看到人体感应打印信息。将手在传感器前约5厘米来回摆动，可看到检测到人体时串口打印 `true`。

```text
entering kernel init...
hilog will init.
[MAIN:D]Main: LOS_Start ...
Entering scheduler
OHOS # hiview init success.
[GPIO:D]LzGpioInit: id 13 is initialized successfully
body: false
body: false
body: false
body: false
body: true
body: true
body: true
body: false
body: false
body: false
```

![](/GPIO/b8_gpio_body/images/586c2f854fb1098868c8ddf4e17f8812.png)

## 注意事项

1. **GPIO方向与电平**：`HCSR501_GPIO` 必须配置为输入，并确保模块输出逻辑电平兼容3.3V。读取结果基于 `IoTGpioGetInputVal` 得到的高/低电平。
2. **触发极性**：默认高电平表示检测到人体。如需反向，可在 `hcsr501_get()` 中调整返回逻辑。
3. **供电与接线**：确保人体感应模块按实训箱接线图连接，供电稳定（一般为5V，信号输出3.3V）。禁止带电插拔。
4. **感应范围**：HC-SR501 通过红外热释电元件检测人体热量变化，建议保持无遮挡，调节模块上的灵敏度与延时电位器以获得最佳结果。
5. **多任务使用**：`hcsr501_init()` 一般调用一次即可，循环读取 `hcsr501_get()` 时建议适当延时，避免串口输出过快。

## 技术规格

- **通信接口**：GPIO（数字输入）
- **GPIO引脚**：`GPIO0_PB5`
- **输出类型**：高/低电平（高电平＝有人体，低电平＝无人）
- **工作电压**：模块供电典型 5V（需参考实训箱配套模块），信号输出3.3V兼容
- **检测对象**：人体运动/热释电源
- **检测距离**：可调，典型 3~5 m（受环境与模块调节影响）
- **响应时间**：毫秒级数字输出，保持时间由模块电位器设定
- **典型应用**：人体存在检测、安防报警、自动照明等场景
