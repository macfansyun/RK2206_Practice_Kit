# RK2206实训箱操作手册—蜂鸣器开发

## 目录

- [RK2206实训箱操作手册—蜂鸣器开发](#rk2206实训箱操作手册蜂鸣器开发)
  - [目录](#目录)
  - [实验内容](#实验内容)
  - [蜂鸣器 API说明](#蜂鸣器-api说明)
    - [函数说明](#函数说明)
      - [(一)蜂鸣器 初始化](#一蜂鸣器-初始化)
      - [(二)蜂鸣器 反初始化](#二蜂鸣器-反初始化)
      - [(三)蜂鸣器 设置状态](#三蜂鸣器-设置状态)
    - [常量说明](#常量说明)
      - [PWM 端口](#pwm-端口)
  - [代码编译](#代码编译)
    - [添加文件](#添加文件)
    - [修改 BUILD.gn 文件](#修改-buildgn-文件)
    - [编译结果](#编译结果)
    - [烧录固件](#烧录固件)
    - [运行结果](#运行结果)
  - [注意事项](#注意事项)
  - [技术规格](#技术规格)

## 实验内容

在RK2206实训箱上使用PWM接口，实现蜂鸣器的编程开发。

## 蜂鸣器 API说明

PWM相关API详见PPT。

### 函数说明

#### (一)蜂鸣器 初始化

| 定义：       | beep_init(void)                                             |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **初始化蜂鸣器模块**                                        |
| **参数：**   | **void**                                                     |
| **返回值：** | **void**                                                     |
| **依赖：**   | **\\code_base\\b3_pwm_beep\\include\\beep.h**               |
| **Tips:**    | **该接口只需调用一次，不支持重复调用。内部会初始化 PWM7_M0 端口** |

#### (二)蜂鸣器 反初始化

| 定义：       | beep_deinit(void)                                            |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **反初始化蜂鸣器模块**                                       |
| **参数：**   | **void**                                                     |
| **返回值：** | **void**                                                     |
| **依赖：**   | **\\code_base\\b3_pwm_beep\\include\\beep.h**               |
| **Tips:**    | **释放PWM资源，停止蜂鸣器控制**                               |

#### (三)蜂鸣器 设置状态

| 定义：       | beep_set(bool state)                                         |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **设置蜂鸣器开关状态**                                       |
| **参数：**   | **`state`: 布尔值，true为开启，false为关闭**                |
| **返回值：** | **void**                                                     |
| **依赖：**   | **\\code_base\\b3_pwm_beep\\include\\beep.h**               |
| **Tips:**    | **开启时频率为500Hz，占空比为50%；关闭时停止PWM输出**         |

### 常量说明

#### PWM 端口

- `EPWMDEV_PWM7_M0`

## 代码编译

### 添加文件

将`~vendor/company_demo/b3_pwm_beep`这整个文件夹拖入`~/application/samples`路径下

![](/PWM/b3_pwm_beep/images/image-7.png)

### 修改 BUILD.gn 文件

修改 `~/application/samples/` 路径下 BUILD.gn 文件，将如下代码添加到对应位置，指定 `beep_example` 参与编译。

```r
"./b3_pwm_beep:beep_example",
```

![](/PWM/b3_pwm_beep/images/image-8.png)

修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下 Makefile 文件，添加 `-lbeep_example` 参与编译。

```r
hardware_LIBS = -lhal_iothardware -lhardware -lshellcmd -lbeep_example
```

![](/PWM/b3_pwm_beep/images/image-9.png)

### 编译结果

![](/PWM/b3_pwm_beep/images/image-11.png)

### 烧录固件

打开烧录工具RKDevTool

![](/PWM/b3_pwm_beep/images/image-20251021170850513.png)

指定两个烧录文件, 编译出的目标文件在 `out/rk2206/lockzhiner-rk2206/images`目录下。
![](/PWM/b3_pwm_beep/images/image-20251021193107443.png)
在RK2206实训箱上上，长按左侧模块控制终端的MaskRom按键（MaskRom按键不要松开），点击ReSet按键（短按，按下后松开，但MaskRom按键不要松开，大约5秒后松开)，烧写工具出现："发现一个MASKROM设备"。如下图所示：
![](/PWM/b3_pwm_beep/images/image-20251021193107443.png)

此时点击"执行"，下载程序，下载完成如下：
![](/PWM/b3_pwm_beep/images/image-20251021193256609.png)

### 运行结果

示例代码编译烧录代码后，按下开发板的RESET按键，蜂鸣器控制单元发出响声
![](/PWM/b3_pwm_beep/images/image-12.png)



## 注意事项

1. **初始化顺序**: 必须先调用 `beep_init()` 再进行状态设置
2. **PWM 通信**: 使用PWM7_M0端口
3. **频率设置**: 开启时频率为500Hz，占空比为50%
4. **电源要求**: 确保蜂鸣器有足够的电源供应
5. **音量控制**: 可通过调整占空比来控制音量大小

## 技术规格

- **通信接口**: PWM
- **PWM 端口**: PWM7_M0
- **工作频率**: 500Hz
- **占空比**: 50%
- **工作电压**: 3.3V-5V
- **工作电流**: 10mA-30mA
- **声音频率**: 500Hz