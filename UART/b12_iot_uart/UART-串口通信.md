# RK2206实训箱操作手册—UART控制开发

小凌派-RK2206 开发板

## 目录

- [RK2206实训箱操作手册—UART控制开发](#rk2206实训箱操作手册uart控制开发)
  - [目录](#目录)
  - [实验内容](#实验内容)
  - [UART API说明](#uart-api说明)
    - [函数说明](#函数说明)
      - [(一)UART 初始化](#一uart-初始化)
      - [(二)UART 取消初始化](#二uart-取消初始化)
      - [(三)UART 读取数据](#三uart-读取数据)
      - [(四)UART 写入数据](#四uart-写入数据)
  - [代码编译](#代码编译)
    - [添加文件](#添加文件)
    - [修改 BUILD.gn 文件](#修改-buildgn-文件)
    - [编译结果](#编译结果)
    - [烧录固件](#烧录固件)
    - [运行结果](#运行结果)
  - [注意事项](#注意事项)
  - [技术规格](#技术规格)

## 实验内容

在RK2206实训箱上使用 **IOT库的UART接口**，实现UART通信编程开发。  
通过配置UART引脚，实现串口数据的收发操作，并在终端查看UART读写结果。

## UART API说明

UART相关API定义在：

```c
base/iot_hardware/peripheral/interfaces/kits/iot_uart.h
```

### 函数说明

#### (一)UART 初始化

| 定义：       | `unsigned int IoTUartInit(unsigned int id, const IotUartAttribute *param)` |
| ------------ | ------------------------------------------------------------ |
| **功能：**   | **初始化指定的UART设备**                                     |
| **参数：**   | **`id`: UART编号（UART0/1/2）； `param`: UART配置参数结构体指针** |
| **返回值：** | **IOT_SUCCESS(成功) / IOT_FAILURE(失败)**                   |
| **依赖：**   | **`/device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite/hal_iot_uart.c`** |
| **Tips:**    | **根据UART ID自动配置引脚复用，如UART1_M0对应GPIO0_PC3/PC2** |

UART引脚复用如下表所示：

| 串口  | id        | TX        | RX        |
| ----- | --------- | --------- | --------- |
| UART0 | EUART0_M0 | GPIO0_PB7 | GPIO0_PB6 |
| UART0 | EUART0_M1 | GPIO0_PC7 | GPIO0_PC6 |
| UART1 | EUART1_M0 | GPIO0_PC3 | GPIO0_PC2 |
| UART1 | EUART1_M1 | GPIO0_PA7 | GPIO0_PA6 |
| UART2 | EUART2_M1 | GPIO0_PB3 | GPIO0_PB2 |

#### (二)UART 取消初始化

| 定义：       | `unsigned int IoTUartDeinit(unsigned int id)` |
| ------------ | ---------------------------------------------- |
| **功能：**   | **释放UART资源，关闭对应设备**                 |
| **参数：**   | **`id`: UART编号**                             |
| **返回值：** | **IOT_SUCCESS / IOT_FAILURE**                 |
| **依赖：**   | **`hal_iot_uart.c`**                          |
| **Tips:**    | **在重新配置UART或结束通信前，需调用此函数释放设备** |

#### (三)UART 读取数据

| 定义：       | `int IoTUartRead(unsigned int id, unsigned char *data, unsigned int dataLen)` |
| ------------ | ----------------------------------------------------------------------------- |
| **功能：**   | **从UART设备中读取指定长度数据**                                              |
| **参数：**   | **`id`: UART编号；`data`: 数据缓冲区；`dataLen`: 读取长度**                   |
| **返回值：** | **实际读取字节数 / IOT_FAILURE**                                             |
| **依赖：**   | **`hal_iot_uart.c`**                                                         |
| **Tips:**    | **该函数为非阻塞读取，需要循环检测或结合任务延时读取**                       |

#### (四)UART 写入数据

| 定义：       | `int IoTUartWrite(unsigned int id, const unsigned char *data, unsigned int dataLen)` |
| ------------ | ------------------------------------------------------------------------------------ |
| **功能：**   | **向UART设备发送指定长度数据**                                                      |
| **参数：**   | **`id`: UART编号；`data`: 待发送数据指针；`dataLen`: 数据长度**                     |
| **返回值：** | **IOT_SUCCESS / IOT_FAILURE**                                                       |
| **依赖：**   | **`hal_iot_uart.c`**                                                                |
| **Tips:**    | **该函数为非阻塞写操作，建议配合延时或事件机制使用**                               |

## 代码编译

### 添加文件

将 `~vendor/lockzhiner/rk2206/sample/b20_iot_uart` 整个文件夹拷贝至 `~/application/samples` 目录下。

![](/UART/b12_iot_uart/images/image-demo.png)

### 修改 BUILD.gn 文件

在 `~/application/samples/BUILD.gn` 文件中，添加以下内容，使 `iot_uart_example` 参与编译：

```r
"./b12_iot_uart:iot_uart_example",
```


修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下的 `Makefile` 文件，添加：

```r
hardware_LIBS = -lhal_iothardware -lhardware -lshellcmd -liot_uart_example
```

### 编译结果

编译完成后可在终端看到如下输出：

![](/UART/b12_iot_uart/images/image-20251021154701056.png)

### 烧录固件

1. 打开 **RKDevTool** 烧录工具。  
2. 指定两个烧录文件，位于路径：  
   `out/rk2206/lockzhiner-rk2206/images/`。  

![](/UART/b12_iot_uart/images/image-20251021170850513.png)

3. 进入 MaskRom 模式：
   - 长按 **MaskRom** 键不放；
   - 短按 **Reset** 键；
   - 等待 5 秒后松开 MaskRom 键；
   - 工具提示 “发现一个MASKROM设备”。
     
![](/UART/b12_iot_uart/images/image-20251021193107443.png)

4. 点击“执行”，等待下载完成。

![](/UART/b12_iot_uart/images/image-20251021193256609.png)

### 运行结果

下载完成后，按下开发板 **RESET** 按键。  
打开串口助手，可看到 UART 收发日志如下：

```
entering kernel init...
hilog will init.
[MAIN:D]Main: LOS_Start ...
Entering scheduler
OHOS # hiview init success.
../../../vendor/lockzhiner/rk2206/samples/b20_iot_uart/iot_uart_example.c, 63: uart write and str(HelloWorld!), len(11)!
../../../vendor/lockzhiner/rk2206/samples/b20_iot_uart/iot_uart_example.c, 72: uart recv and str(HelloWorld!), len(11)
../../../vendor/lockzhiner/rk2206/samples/b20_iot_uart/iot_uart_example.c, 63: uart write and str(HelloWorld!), len(11)!
../../../vendor/lockzhiner/rk2206/samples/b20_iot_uart/iot_uart_example.c, 72: uart recv and str(HelloWorld!), len(11)
```

## 注意事项

1. **UART连线要求**：需使用杜邦线将 **TX ↔ RX** 相互连接。
2. **波特率设置**：示例默认 115200，可在 `attr.baudRate` 修改。
3. **任务模式**：`IoTUartWrite` 为非阻塞发送，循环中应加入 `LOS_Msleep()`。
4. **多串口支持**：UART0~UART2 均可使用，需对应修改引脚表。

## 技术规格

- **通信接口**：UART（全双工）
- **波特率范围**：9600 ~ 921600 bps
- **数据位**：5/6/7/8 位
- **停止位**：1 或 2 位
- **校验模式**：None / Even / Odd
- **流控**：None
- **默认参数**：115200, 8N1
