# RK2206实训箱操作手册—RC522开发

MFRC522 RFID 模块（SPI 接口）

## 目录

- [RK2206实训箱操作手册—RC522开发](#rk2206实训箱操作手册rc522开发)
  - [目录](#目录)
  - [实验内容](#实验内容)
  - [RC522 API说明](#rc522-api说明)
    - [函数说明](#函数说明)
      - [(一)RC522 初始化](#一rc522-初始化)
      - [(二)RC522 寻卡](#二rc522-寻卡)
      - [(三)RC522 防冲突](#三rc522-防冲突)
    - [常量和寄存器说明](#常量和寄存器说明)
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

在 RK2206 实训箱上通过 **SPI 接口** 与 **MFRC522 RFID 模块** 通信，实现 RFID 卡片的识别与卡号读取功能。

---

## RC522 API说明

SPI 相关 API 详见 PPT。

---

### 函数说明

#### (一)RC522 初始化

| 定义：       | `void rc522_init(void)` |
| ------------ | ---------------------- |
| **功能：**   | 初始化 RC522 模块及 SPI 通信接口 |
| **参数：**   | 无 |
| **返回值：** | 无 |
| **依赖：**   | `\\code_base\\b11_spi_rfid\\include\\rc522.h` |
| **Tips:**    | 初始化 SPI2 通道（1MHz），并设置 RC522 进入待机模式。 |

---

#### (二)RC522 寻卡

| 定义：       | `unsigned char rc522_request(unsigned char reqMode, unsigned char *tagType)` |
| ------------ | ---------------------------------------------------------------------------- |
| **功能：**   | 发送寻卡命令，检测是否存在卡片 |
| **参数：**   | `reqMode`：寻卡模式（一般为 `PICC_REQIDL`）<br>`tagType`：卡片类型返回数组 |
| **返回值：** | 状态码（`MI_OK` 表示成功） |
| **依赖：**   | `\\code_base\\b11_spi_rfid\\include\\rc522.h` |
| **Tips:**    | 若检测到卡片，则后续可进行防冲突处理。 |

---

#### (三)RC522 防冲突

| 定义：       | `unsigned char rc522_anticoll(unsigned char *serNum)` |
| ------------ | ----------------------------------------------------- |
| **功能：**   | 防冲突处理，读取卡片的唯一序列号（UID） |
| **参数：**   | `serNum`：存储卡片 UID 的数组 |
| **返回值：** | 状态码（`MI_OK` 表示成功） |
| **依赖：**   | `\\code_base\\b11_spi_rfid\\include\\rc522.h` |
| **Tips:**    | 用于多卡情况下识别唯一卡号。 |

---

### 常量和寄存器说明

| 名称 | 地址 | 功能描述 |
| ---- | ---- | -------- |
| `CommandReg` | 0x01 | 命令寄存器 |
| `ComIEnReg` | 0x02 | 中断使能控制 |
| `ComIrqReg` | 0x04 | 中断请求寄存器 |
| `ErrorReg` | 0x06 | 错误状态寄存器 |
| `Status1Reg` | 0x07 | 通信状态寄存器 |
| `FIFODataReg` | 0x09 | FIFO 数据读写寄存器 |
| `BitFramingReg` | 0x0D | 位帧调整寄存器 |
| `ModeReg` | 0x11 | 模式控制寄存器 |
| `TxControlReg` | 0x14 | 天线驱动控制寄存器 |
| `VersionReg` | 0x37 | 固件版本号寄存器 |

---

## 代码编译

### 添加文件

将`~vendor/company_demo/b11_spi_rfid`这整个文件夹拖入`~/application/samples`路径下

![](/SPI/b11_spi_rfid/images/image-rfid.png)

### 修改 BUILD.gn 文件

修改 `~/application/samples/` 路径下 BUILD.gn 文件，将如下代码添加到对应位置，指定 `rfid_example` 参与编译。

```r
"./b11_spi_rfid:rfid_example",
```

修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下 Makefile 文件，添加 `-rfid_example` 参与编译。

```r
hardware_LIBS = -lhal_iothardware -lhardware -lshellcmd -rfid_example
```

### 编译结果

![](/SPI/b11_spi_rfid/images/image-20251021154701056.png)

### 烧录固件

打开烧录工具RKDevTool

![](/SPI/b11_spi_rfid/images/image-20251021170153891.png)

指定两个烧录文件, 编译出的目标文件在 `out/rk2206/lockzhiner-rk2206/images`目录下。
![](/SPI/b11_spi_rfid/images/image-20251021170850513.png)
在RK2206实训箱上上，长按左侧模块控制终端的MaskRom按键（MaskRom按键不要松开），点击ReSet按键（短按，按下后松开，但MaskRom按键不要松开，大约5秒后松开)，烧写工具出现：“发现一个MASKROM设备”。如下图所示：

![](/SPI/b11_spi_rfid/images/image-20251021193107443.png)

此时点击“执行”，下载程序，下载完成如下：

![](/SPI/b11_spi_rfid/images/image-20251021193256609.png)

### 运行结果

示例代码编译烧录代码后，按下开发板的RESET按键，连接串口，可以看到RFID卡ID。

```
entering kernel init...
hilog will init.
[MAIN:D]Main: LOS_Start ...
Entering scheduler
OHOS # hiview init success.
[GPIO:D]LzGpioInit: id 16 is initialized successfully
[GPIO:D]LzGpioInit: id 17 is initialized successfully
[GPIO:D]LzGpioInit: id 18 is initialized successfully
[GPIO:D]LzGpioInit: id 19 is initialized successfully
New RFID: 1CCC9891
New RFID: 52FDFC69
New RFID: 92FDFC69
```



## 注意事项

1. **初始化顺序**:先 `rc522_init()` 再调用` rc522_request()`
2. **SPI 通信**: 建议 `1MHz`，避免过快通信导致数据错误
3. **天线控制**: 确保 `TxControlReg` 使能天线输出
4. **卡片距离**: 推荐感应距离 ≤ 3cm，避免识别不稳定。

## 技术规格

- **通信接口**: SPI
- **SPI 速率**: 1Mhz
- **工作电压**: 3.3V 