# 小凌派-RK2206开发板OpenHarmonyOS外设开发-超声波测距传感器

## 实验内容

本例程演示如何在小凌派-RK2206开发板上使用IOT库的GPIO接口，进行超声波测距传感器编程开发。

<img src="https://github.com/WiBeeBaBu/rk2206_practice_kit/master/Src/b10_gpio_distance/images/lockzhiner-rk2206.jpg" width="100%" style="max-width: 800px;" />

## 程序设计

### GPIO API分析

#### 头文件

```c    
base/iot_hardware/peripheral/interfaces/kits/iot_gpio.h
```

#### 初始化GPIO设备：IoTGpioInit

```c
unsigned int IoTGpioInit(unsigned int id);
```

**描述：**

初始化GPIO设备。

**参数：**

| 参数 | 类型         | 描述       |
| ---- | ------------ | ---------- |
| id   | unsigned int | GPIOID编号 |

**返回值：**


| 返回值      | 描述 |
| ----------- | ---- |
| IOT_SUCCESS | 成功 |
| IOT_FAILURE | 失败 |

**实现：**

`hal_iot_gpio.c`文件在`device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite`目录下。

```c
unsigned int IoTGpioInit(unsigned int id)
{
    unsigned int ret = 0;

    if (id > GPIO0_PC7 && id != GPIO1_PD0) {
        PRINT_ERR("id(%d) > %d || id(%d) != %d\n", id, GPIO0_PC7, GPIO1_PD0);
        return IOT_FAILURE;
    }

    ret = LzGpioInit(id);
    if (ret != LZ_HARDWARE_SUCCESS) {
        return IOT_FAILURE;
    }

    ret = PinctrlSet(id, MUX_FUNC0, PULL_KEEP, DRIVE_LEVEL0);
    if (ret != LZ_HARDWARE_SUCCESS) {
        return IOT_FAILURE;
    }

    return IOT_SUCCESS;
}
```

#### 取消初始化GPIO设备：IoTGpioDeinit

```c
unsigned int IoTGpioDeinit(unsigned int id);
```

**描述：**

取消初始化GPIO设备。

**参数：**

| 参数 | 类型         | 描述       |
| ---- | ------------ | ---------- |
| id   | unsigned int | GPIOID编号 |

**返回值：**


| 返回值      | 描述 |
| ----------- | ---- |
| IOT_SUCCESS | 成功 |
| IOT_FAILURE | 失败 |

**实现：**

`hal_iot_gpio.c`文件在`device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite`目录下。

```c
unsigned int IoTGpioDeinit(unsigned int id)
{
    unsigned int ret = 0;

    if (id > GPIO0_PC7 && id != GPIO1_PD0) {
        PRINT_ERR("id(%d) > %d || id(%d) != %d\n", id, GPIO0_PC7, GPIO1_PD0);
        return IOT_FAILURE;
    }

    ret = LzGpioDeinit(id);
    if (ret != LZ_HARDWARE_SUCCESS) {
        return IOT_FAILURE;
    }

    return IOT_SUCCESS;
}
```

#### 设置GPIO引脚的方向：IoTGpioSetDir

```c
unsigned int IoTGpioSetDir(unsigned int id, IotGpioDir dir);
```

**描述：**

设置GPIO引脚的方向。

**参数：**

| 参数 | 类型         | 描述       |
| ---- | ------------ | ---------- |
| id   | unsigned int | GPIOID编号 |
| dir  | IotGpioDir   | GPIO方向   |

其中，dir对应于如下表所示：

| IotGpioDir       | 描述 |
| ---------------- | ---- |
| IOT_GPIO_DIR_IN  | 输入 |
| IOT_GPIO_DIR_OUT | 输出 |


**返回值：**


| 返回值      | 描述 |
| ----------- | ---- |
| IOT_SUCCESS | 成功 |
| IOT_FAILURE | 失败 |

**实现：**

`hal_iot_gpio.c`文件在`device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite`目录下。

```c
unsigned int IoTGpioSetDir(unsigned int id, IotGpioDir dir)
{
    unsigned int ret = 0;

    if (id > GPIO0_PC7 && id != GPIO1_PD0) {
        PRINT_ERR("id(%d) > %d || id(%d) != %d\n", id, GPIO0_PC7, GPIO1_PD0);
        return IOT_FAILURE;
    }

    ret = LzGpioSetDir(id, (LzGpioDir)dir);
    if (ret != LZ_HARDWARE_SUCCESS) {
        return IOT_FAILURE;
    }   

    return IOT_SUCCESS;
}
```

#### 获取GPIO引脚的方向：IoTGpioGetDir

```c
unsigned int IoTGpioGetDir(unsigned int id, IotGpioDir *dir);
```

**描述：**

获取GPIO引脚的方向。

**参数：**

| 参数 | 类型         | 描述         |
| ---- | ------------ | ------------ |
| id   | unsigned int | GPIOID编号   |
| dir  | IotGpioDir * | GPIO方向指针 |

**返回值：**


| 返回值      | 描述 |
| ----------- | ---- |
| IOT_SUCCESS | 成功 |
| IOT_FAILURE | 失败 |

**实现：**

`hal_iot_gpio.c`文件在`device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite`目录下。

```c
unsigned int IoTGpioGetDir(unsigned int id, IotGpioDir *dir)
{
    unsigned int ret = 0;

    if (id > GPIO0_PC7 && id != GPIO1_PD0) {
        PRINT_ERR("id(%d) > %d || id(%d) != %d\n", id, GPIO0_PC7, GPIO1_PD0);
        return IOT_FAILURE;
    }

    ret = LzGpioGetDir(id, (LzGpioDir *)dir);
    if (ret != LZ_HARDWARE_SUCCESS) {
        return IOT_FAILURE;
    }   

    return IOT_SUCCESS;
}
```

#### 设置GPIO引脚的输出电平值：IoTGpioSetOutputVal

```c
unsigned int IoTGpioSetOutputVal(unsigned int id, IotGpioValue val);
```

**描述：**

设置GPIO引脚的输出电平值。

**参数：**

| 参数 | 类型         | 描述       |
| ---- | ------------ | ---------- |
| id   | unsigned int | GPIOID编号 |
| val  | IotGpioValue | 电平       |

其中，val对应于如下表所示：

| IotGpioValue    | 描述   |
| --------------- | ------ |
| IOT_GPIO_VALUE0 | 低电平 |
| IOT_GPIO_VALUE1 | 高电平 |

**返回值：**


| 返回值      | 描述 |
| ----------- | ---- |
| IOT_SUCCESS | 成功 |
| IOT_FAILURE | 失败 |

**实现：**

`hal_iot_gpio.c`文件在`device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite`目录下。

```c
unsigned int IoTGpioSetOutputVal(unsigned int id, IotGpioValue val)
{
    unsigned int ret = 0;

    if (id > GPIO0_PC7 && id != GPIO1_PD0) {
        PRINT_ERR("id(%d) > %d || id(%d) != %d\n", id, GPIO0_PC7, GPIO1_PD0);
        return IOT_FAILURE;
    }

    ret = LzGpioSetVal(id, (LzGpioValue)val);
    if (ret != LZ_HARDWARE_SUCCESS) {
        return IOT_FAILURE;
    }   

    return IOT_SUCCESS;
}
```

#### 获取GPIO引脚的输出电平值：IoTGpioGetOutputVal

```c
unsigned int IoTGpioGetOutputVal(unsigned int id, IotGpioValue *val);
```

**描述：**

获取GPIO引脚的输出电平值。

**参数：**

| 参数 | 类型           | 描述       |
| ---- | -------------- | ---------- |
| id   | unsigned int   | GPIOID编号 |
| val  | IotGpioValue * | 电平指针   |

**返回值：**


| 返回值      | 描述 |
| ----------- | ---- |
| IOT_SUCCESS | 成功 |
| IOT_FAILURE | 失败 |

**实现：**

`hal_iot_gpio.c`文件在`device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite`目录下。

```c
unsigned int IoTGpioGetOutputVal(unsigned int id, IotGpioValue *val)
{
    unsigned int ret = 0;

    if (id > GPIO0_PC7 && id != GPIO1_PD0) {
        PRINT_ERR("id(%d) > %d || id(%d) != %d\n", id, GPIO0_PC7, GPIO1_PD0);
        return IOT_FAILURE;
    }

    ret = LzGpioGetVal(id, (LzGpioValue *)val);
    if (ret != LZ_HARDWARE_SUCCESS) {
        return IOT_FAILURE;
    }   

    return IOT_SUCCESS;
}
```

#### 获取GPIO引脚的输入电平值：IoTGpioGetInputVal

```c
unsigned int IoTGpioGetInputVal(unsigned int id, IotGpioValue *val);
```

**描述：**

获取GPIO引脚的输入电平值。

**参数：**

| 参数 | 类型           | 描述       |
| ---- | -------------- | ---------- |
| id   | unsigned int   | GPIOID编号 |
| val  | IotGpioValue * | 电平指针   |

**返回值：**


| 返回值      | 描述 |
| ----------- | ---- |
| IOT_SUCCESS | 成功 |
| IOT_FAILURE | 失败 |

**实现：**

`hal_iot_gpio.c`文件在`device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite`目录下。

```c
unsigned int IoTGpioGetInputVal(unsigned int id, IotGpioValue *val)
{
    unsigned int ret = 0;

    if (id > GPIO0_PC7 && id != GPIO1_PD0) {
        PRINT_ERR("id(%d) > %d || id(%d) != %d\n", id, GPIO0_PC7, GPIO1_PD0);
        return IOT_FAILURE;
    }

    ret = LzGpioGetVal(id, (LzGpioValue *)val);
    if (ret != LZ_HARDWARE_SUCCESS) {
        return IOT_FAILURE;
    }   

    return IOT_SUCCESS;
}
```

#### 启用GPIO引脚的中断功能：IoTGpioRegisterIsrFunc

```c
unsigned int IoTGpioRegisterIsrFunc(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity, GpioIsrCallbackFunc func, char *arg);
```

**描述：**

启用GPIO引脚的中断功能。

**参数：**

| 参数        | 类型                | 描述             |
| ----------- | ------------------- | ---------------- |
| id          | unsigned int        | GPIOID编号       |
| intType     | IotGpioIntType      | 中断模式         |
| intPolarity | IotGpioIntPolarity  | 中断极性         |
| func        | GpioIsrCallbackFunc | 中断回调函数     |
| arg         | char *              | 中断回调函数参数 |

**返回值：**


| 返回值      | 描述 |
| ----------- | ---- |
| IOT_SUCCESS | 成功 |
| IOT_FAILURE | 失败 |

**实现：**

`hal_iot_gpio.c`文件在`device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite`目录下。

```c
unsigned int IoTGpioRegisterIsrFunc(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity, GpioIsrCallbackFunc func, char *arg)
{
    unsigned int ret = 0;
    LzGpioIntType type;
    
    if (id > GPIO0_PC7 && id != GPIO1_PD0) {
        PRINT_ERR("id(%d) > %d || id(%d) != %d\n", id, GPIO0_PC7, GPIO1_PD0);
        return IOT_FAILURE;
    }

    if (intType == IOT_INT_TYPE_LEVEL && intPolarity == IOT_GPIO_EDGE_FALL_LEVEL_LOW)
        type = LZGPIO_INT_LEVEL_LOW;
    else if (intType == IOT_INT_TYPE_LEVEL && intPolarity == IOT_GPIO_EDGE_RISE_LEVEL_HIGH)
        type = LZGPIO_INT_LEVEL_HIGH;
    else if (intType == IOT_INT_TYPE_EDGE && intPolarity == IOT_GPIO_EDGE_FALL_LEVEL_LOW)
        type = LZGPIO_INT_EDGE_FALLING;
    else if (intType == IOT_INT_TYPE_EDGE && intPolarity == IOT_GPIO_EDGE_RISE_LEVEL_HIGH)
        type = LZGPIO_INT_EDGE_RISING;
    else if (intType == IOT_INT_TYPE_EDGE && intPolarity == IOT_GPIO_EDGE_BOTH_TYPE)
        type = LZGPIO_INT_EDGE_BOTH;
    else
        return IOT_FAILURE;

    ret = LzGpioRegisterIsrFunc(id, type, (GpioIsrFunc)func, arg);
    if (ret != LZ_HARDWARE_SUCCESS) {
        return IOT_FAILURE;
    }  

    return IOT_SUCCESS;
}
```

#### 禁用GPIO引脚的中断功能：IoTGpioUnregisterIsrFunc

```c
unsigned int IoTGpioUnregisterIsrFunc(unsigned int id);
```

**描述：**

禁用GPIO引脚的中断功能。

**参数：**

| 参数 | 类型         | 描述       |
| ---- | ------------ | ---------- |
| id   | unsigned int | GPIOID编号 |

**返回值：**


| 返回值      | 描述 |
| ----------- | ---- |
| IOT_SUCCESS | 成功 |
| IOT_FAILURE | 失败 |

**实现：**

`hal_iot_gpio.c`文件在`device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite`目录下。

```c
unsigned int IoTGpioUnregisterIsrFunc(unsigned int id)
{
    unsigned int ret = 0;

    if (id > GPIO0_PC7 && id != GPIO1_PD0) {
        PRINT_ERR("id(%d) > %d || id(%d) != %d\n", id, GPIO0_PC7, GPIO1_PD0);
        return IOT_FAILURE;
    }

    ret = LzGpioUnregisterIsrFunc(id);
    if (ret != LZ_HARDWARE_SUCCESS) {
        return IOT_FAILURE;
    }   

    return IOT_SUCCESS;
}
```

#### 屏蔽GPIO引脚的中断功能：IoTGpioSetIsrMask

```c
unsigned int IoTGpioSetIsrMask(unsigned int id, unsigned char mask);
```

**描述：**

屏蔽GPIO引脚的中断功能。

**参数：**

| 参数 | 类型          | 描述                             |
| ---- | ------------- | -------------------------------- |
| id   | unsigned int  | GPIOID编号                       |
| mask | unsigned char | 中断掩码 1：屏蔽开启 0：屏蔽关闭 |

**返回值：**


| 返回值      | 描述 |
| ----------- | ---- |
| IOT_SUCCESS | 成功 |
| IOT_FAILURE | 失败 |

**实现：**

`hal_iot_gpio.c`文件在`device/rockchip/rk2206/adapter/hals/iot_hardware/wifiiot_lite`目录下。

```c
unsigned int IoTGpioSetIsrMask(unsigned int id, unsigned char mask)
{
    unsigned int ret = 0;

    if (id > GPIO0_PC7 && id != GPIO1_PD0) {
        PRINT_ERR("id(%d) > %d || id(%d) != %d\n", id, GPIO0_PC7, GPIO1_PD0);
        return IOT_FAILURE;
    }

    if (!mask)
    {
        ret = LzGpioEnableIsr(id);
        if (ret != LZ_HARDWARE_SUCCESS) {
            return IOT_FAILURE;
        } 
    }
    else
    {
        ret = LzGpioDisableIsr(id);
        if (ret != LZ_HARDWARE_SUCCESS) {
            return IOT_FAILURE;
        } 
    }
    
    return IOT_SUCCESS;
}
```

### 超声波测距传感器 API分析

#### 头文件

```c
hcsr04.h
```

#### 初始化超声波测距传感器：hcsr04_init

```c
void hcsr04_init(void);
```

**描述：**

初始化超声波测距传感器。

**实现：**

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

#### 反初始化超声波测距传感器：hcsr04_deinit

```c
void hcsr04_deinit(void);
```

**描述：**

反初始化超声波测距传感器。

**实现：**

```c
void hcsr04_deinit(void)
{
    IoTGpioDeinit(ECHO_GPIO);
    IoTGpioDeinit(TRIG_GPIO);
}
```

#### 设置超声波测距传感器值：hcsr04_get

```c
double hcsr04_get(void);
```

**描述：**

设置超声波测距传感器值。

**返回值：**


| 返回值 | 类型   | 描述         |
| ------ | ------ | ------------ |
| 距离。 | double | 单位：厘米。 |

**实现：**

```c
double hcsr04_get(void)
{
    if (echo_data.flag == ECHO_FLAG_CAPTURE_RISE) {
        hcsr04_send_trig();
    }

    /* 等待下降沿 */
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
    /* 距离 = 时间差 * 340米/秒 / 2(超时波来回2次) * 100厘米/米 */
    double data = (double)echo_data.pulse_width / (double)ECHO_TIMER_FREQ * 170.0 * 100.0;

    return data;
}
```

### 软件设计

**主要代码分析**

在`distance_example`函数中，创建一个任务。

```c
void distance_example(void)
{
    TSK_INIT_PARAM_S task = {0};

    task.pfnTaskEntry = (TSK_ENTRY_FUNC)gpio_distance_thread;
    task.uwStackSize  = 1024 * 10;
    task.pcName       = "gpio_distance_thread";
    task.uwArg        = NULL;
    task.usTaskPrio   = 10;
    LOS_TaskCreate(&m_thread_id, &task);
}
```

`gpio_distance_thread`任务中调用超声波测距传感器接口获取超声波测距传感器值。

```c
static void gpio_distance_thread(void *args)
{
    hcsr04_init(); // distance 初始化

    while (1) {
        double data = hcsr04_get();
        printf("distance: %.2f\r\n", data);
        LOS_Msleep(1000);
    }
}
```

## 编译调试

### 修改 BUILD.gn 文件

修改 `vendor/lockzhiner/rk2206/sample` 路径下 BUILD.gn 文件，指定 `distance_example` 参与编译。

```r
"./f10_gpio_distance:distance_example",
```

修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下 Makefile 文件，添加 `-ldistance_example` 参与编译。

```r
hardware_LIBS = -lhal_iothardware -lhardware -lshellcmd -ldistance_example
```

### 运行结果

示例代码编译烧录代码后，按下开发板的RESET按键，连接串口，可以看到距离信息。

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
distance: 204.08
distance: 204.12
distance: 204.56
distance: 204.59
distance: 204.18
distance: 204.53
```

