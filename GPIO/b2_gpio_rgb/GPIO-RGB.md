# RK2206实训箱操作手册—OpenHarmonyOS外设开发-RGB

OpenHarmonyOS外设开发-RGB

## 目录

- [RK2206实训箱操作手册—OpenHarmonyOS外设开发-RGB](#rk2206实训箱操作手册openharmonyos外设开发-rgb)
  - [目录](#目录)
  - [实验内容](#实验内容)
  - [RGB API说明](#rgb-api说明)
    - [函数说明](#函数说明)
      - [(一)、RGB 初始化](#一rgb-初始化)
      - [(二)、RGB 反初始化](#二rgb-反初始化)
      - [(三)、RGB灯的几种颜色状态](#三rgb灯的几种颜色状态)
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

本例程演示如何在小凌派-RK2206开发板上使用IOT库的GPIO接口，进行RGB编程开发。

## RGB API说明

GPIO相关API详见PPT（如 `IoTGpioInit`、`IoTGpioDeinit`、`IoTGpioGetInputVal` 等），本节重点说明RGB封装接口。

### 函数说明

#### (一)、RGB 初始化

| 定义：       | `void rgb_init(rgb_t *rgb)`|
| ------------ | ----------------------------------------------- |
| **功能：**   | **初始化RGB模块，配置对应GPIO引脚**     |
| **参数：**   | **void**                                        |
| **返回值：** | **void**                                        |
| **依赖：**   | **`\\code_base\\b2_gpio_rgb\\include\\rgb.h`** |
| **Tips:**    | **内部会调用 `IoTGpioInit(rgb->red_gpio);` 完成GPIO初始化** |

#### (二)、RGB 反初始化

| 定义：       | `void rgb_deinit(rgb_t *rgb)`                         |
| ------------ | ----------------------------------------------- |
| **功能：**   | **反初始化RGB模块，释放GPIO资源**       |
| **参数：**   | **void**                                        |
| **返回值：** | **void**                                        |
| **依赖：**   | **`\\code_base\\b2_gpio_rgb\\include\\rgb.h`** |
| **Tips:**    | **在不再使用RGB时调用，避免占用多余GPIO资源** |

#### (三)、RGB灯的几种颜色状态

| 定义：       | `void rgb_lights_set(rgb_t *rgb, rgb_state_e state)`                      |
| ------------ | ----------------------------------------------- |
| **功能：**   | **更改RGB灯的颜色状态**                  |
| **参数：**   | **void**                                        |
| **返回值：** | **void**                         |
| **依赖：**   | **`\\code_base\\b2_gpio_rgb\\include\\rgb.h`** |
| **Tips:**    | **内部通过 ` IoTGpioSetOutputVal(rgb->green_gpio, IOT_GPIO_VALUE1)` 获取引脚号(rgb->green_gpio)和电平值(IOT_GPIO_VALUE1),改变颜色** |


### 常量说明

#### GPIO 引脚
根据示例代码，RGB LED灯对应的GPIO引脚如下：
|颜色|GPIO端口|描述|
|-|-|-|
|红色LED|GPIO0_PA4|红色灯控制引脚|
|绿色LED|GPIO0_PA2|绿色灯控制引脚|
|蓝色LED|GPIO0_PA3|蓝色灯控制引脚|

## 代码编译

### 添加文件

将 `~vendor/company_demo/b7_gpio_fire` 这整个文件夹拖入 `~/application/samples` 路径下。

![](/GPIO/b2_gpio_rgb/images/500f7427-d0f5-4784-a5d4-65ddbe058724.png)

### 修改 BUILD.gn 文件

修改 `~/application/samples/` 路径下 BUILD.gn 文件，将如下代码添加到对应位置，指定 `fire_example` 参与编译。

```r
"./b2_gpio_rgb:rgb_example",
```

修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下 Makefile 文件，添加 `-lfire_example` 参与编译。

```r
hardware_LIBS = -lhal_iothardware -lhardware -lshellcmd -lrgb_example
```

### 编译结果

![](/GPIO/b2_gpio_rgb/images/62944df3881d43da0a226ebf933d43f0.png)

## 烧录固件

打开烧录工具RKDevTool。
![](/GPIO/b2_gpio_rgb/images/image-20251021170153891.png)
指定两个烧录文件，编译出的目标文件在 `out/rk2206/lockzhiner-rk2206/images` 目录下。
![](/GPIO/b2_gpio_rgb/images/image-20251021170850513.png)
在RK2206实训箱上，长按左侧模块控制终端的 MaskRom 按键（MaskRom 按键不要松开），点击 Reset 按键（短按，按下后松开，但 MaskRom 按键不要松开，大约5秒后松开)。
![](/GPIO/b2_gpio_rgb/images/image-20251021193107443.png)
烧写工具出现："发现一个MASKROM设备" 提示后，点击“执行”，开始下载程序。
![](/GPIO/b2_gpio_rgb/images/image-20251021193256609.png)


## 运行结果

示例代码编译烧录代码后，按下开发板的RESET按键，可以观察RGB灯由绿、蓝、红、白、灭的交换。

![](/GPIO/b2_gpio_rgb/images/b1b632f46b8f68cc486f798669101d57.jpg)
![](/GPIO/b2_gpio_rgb/images/4bb3170a2655fe824139d0e0cf550841.jpg)
![](/GPIO/b2_gpio_rgb/images/15a99dac243e5ccc7a3cce2ff193c4bf.jpg)
![](/GPIO/b2_gpio_rgb/images/2bbecb7472933e5cf4111a71b5a04f1f.jpg)
![](/GPIO/b2_gpio_rgb/images/d4c0f85d0f1823a8e1db8bf4becea4fe.jpg)

## 注意事项

1. **初始化顺序**：必须先调用 rgb_init() 并传入正确的GPIO引脚配置，再进行状态设置
2. **结构体传参**：RGB控制函数均需传入 rgb_t 结构体指针，该结构体需预先定义并赋值正确的GPIO引脚号：
```
typedef struct {
    uint32_t red_gpio;   // 红色LED GPIO引脚
    uint32_t green_gpio; // 绿色LED GPIO引脚
    uint32_t blue_gpio;  // 蓝色LED GPIO引脚
} rgb_t;
```
3. **GPIO电平逻辑**：

   - `IOT_GPIO_VALUE1`：高电平（点亮LED）

   - `IOT_GPIO_VALUE0`：低电平（熄灭LED）

   - 实训箱RGB LED为共阴极设计，高电平有效

4. **引脚冲突**：GPIO0_PA2、GPIO0_PA3、GPIO0_PA4为RGB LED专用引脚，请勿在其他功能中重复初始化或占用。
5. **白灯显示**：白灯状态为同时点亮红、绿、蓝三色LED，实际显示效果为白色（取决于LED封装与混光效果）
6. **资源释放**：程序退出或不再使用RGB灯时，建议调用 rgb_deinit() 释放GPIO资源

## 技术规格

- **通信接口**:	GPIO（通用输入输出）
- **控制方式**:	独立引脚高低电平控制
- **GPIO端口**:	`GPIO0_PA2`（绿）、`GPIO0_PA3`（蓝）、`GPIO0_PA4`（红）
- **电平逻辑**:	高电平有效（1-点亮，0-熄灭）
- **LED类型**:	共阴极RGB三色LED
- **显示颜色**:	红、绿、蓝、白、灭（共5种状态）
- **工作电压**:	3.3V（RK2206 GPIO输出电压）
- **限流电阻**:	板载已集成，无需外部电阻
- **响应时间**:	微秒级（GPIO翻转速度）
- **典型应用**:	状态指示、呼吸灯、颜色混合实验
