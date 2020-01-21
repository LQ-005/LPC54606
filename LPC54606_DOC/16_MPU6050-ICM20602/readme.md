[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# MPU6050-ICM20602例程

>ICM20602是MPU6050的改进版，精度更高，噪声更低，支持SPI传输方式。当使用IIC接线方式时，MPU6050和ICM20602的程序可以通用。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [IIC简介](###IIC简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)

## 硬件电路

母板上的陀螺仪接口和摄像头的SCCB接口公用（摄像头只是初始化时占用，初始化完成后就不会使用了，因此如果使用摄像头和陀螺仪，需要先初始化摄像头）
<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa45902693175f4630aaa4fce2721dac9?method=download&shareKey=f48e4cdb380176155aff7a9173e96457">
</div>

## 软件实现

### IIC简介

>LPC54606 有9个FlexCOM口，这九个FlexCOM口都可以变身成为IIC接口。

IIC是一种通信协议，由数据线SDA和时钟线SCL组成,支持一个主机多个从机通信模式（一个IIC总线上可以挂载多个地址不同的从机）。

* IIC地址

>每一个IIC器件都有一个七位的地址，有些同学可能会问，不是八位地址吗？实际上主机向从机发送地址时实际发送的是7位地址+1位读写（0写，1读）组成的byte，例如MPMU6050的IIC器件地址就是0x68,但是实际上如果主机要写MPU6050寄存器，会发送写地址0xD0，要读会发送读地址0xD1。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBc164558871bc144cab21d67b65705e60?method=download&shareKey=72a701c2c5a635958702d0230870c767">
</div>

* IIC起始、停止信号

>IIC空闲时，SDA和SCL都为高电平（虽然LCP54606支持3.4MHz的高速IIC，但是MPU6050不支持，这里不进行讲解），这也是IIC总线需要上拉电阻的原因。 

开始信号：当SCL为高时，SDA从高电平跳变到低电平；  
停止信号：当SCL为高时，SDA从低电平跳变到高电平；

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB73166288fc2e488e173025130496fdd2?method=download&shareKey=3c629c7ccba0926d2416e69116161e6e">
</div>

* 应答信号

>IIC主机向从机发送数据，发送完一个byte后，主机会拉高数据线SDA，从机如果收到数据，会拉低数据线SDA，主机检测到数据线SDA为低即收到应答信号。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB3f9fee78fb2457b82766d3eea5b929ef?method=download&shareKey=907c4b4ae6a07f884a7b80bf1beffdab">
</div>

* IIC读取一个寄存器

IIC数据线只能在时钟SCL为低时发生跳变
<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBc2cfaaa89ec3d8282d74886b6d424d4f?method=download&shareKey=0d0d17456fab3efe663ff5885724b6ac">
</div>

IIC读取寄存器：主机先发送起始信号，然后发送写地址，收到从机应答信号后，接着发送寄存器地址，收到从机应答后再次发送起始信号，发送读地址，从机应答后会发送寄存器中的数据，主机收到数据后不应答，发送停止信号。
<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB7142a3f90ae9b0b1402f3b656994f2ea?method=download&shareKey=6f3560b5f2e857dd844a22806fd90449">
</div>

* IIC写一个寄存器

IIC写入寄存器：主机先发送起始信号，然后发送写地址，收到从机应答信号后，接着发送寄存器地址，收到从机应答后再次发送要写入的数据，从机接收到数据后发送应答信号，主机发送停止信号。
<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB93ee94dcb1b67d55a1dca2fc6bc125ce?method=download&shareKey=6e78785ea7debd94d1ffb816e7e81b30">
</div>

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB11853de45be2b9298d26d8397b6b80e0?method=download&shareKey=174736bf69d3e29e76a9fd9da715d526">
</div>

* 硬件IIC和软件IIC区别

上面介绍了IIC的时序，软件IIC就是自己控制GPIO输出，模拟IIC的时序，而硬件IIC则是芯片内部集成了硬件IIC发生器，只需要操作对应单片机寄存器即可。

LPC54606 有9个FlexCOM口，这九个FlexCOM口都可以变身成为硬件IIC接口。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB6a90714a098a04f729b6370cd2523dbc?method=download&shareKey=d3eb345d322a58dc313061b5cacdc1fa">
</div>

模拟IIC任意IO口都可以

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB810c1f7eaa52fadd812226f1cf2abe20?method=download&shareKey=c45a7e429b0eead9767f80b34e63842f">
</div>

速度上，对于最大通信速度400K的MPU6050/ICM20602来说，软硬件IIC速度没有差别，不过模拟IIC想要调节速率需要借助示波器工具。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB76389fd37de7da0224ef48ac250884cf?method=download&shareKey=fec1a9e02f17a9bf408d814042bedc2c">
</div>

硬件IIC地址用的是7位地址，龙邱的模拟IIC地址用的是写地址，即硬件IIC地址乘以2（MPU6050使用硬件IIC时地址传入0x68，使用龙邱模拟IIC时传入地址0xD0）。

### 常用API

>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>

* 硬件IIC初始化

```c
/*!
  * @brief    IIC初始化
  *
  * @param    base      ：  IIC端口号  I2C0 - I2C8
  * @param    baudrate  ：  波特率 
  *
  * @return   无
  *
  * @note     这里实际是将flexcom口用作IIC，如果使用了flexcom0当作IIC0，那么flexcom0 就不可用作串口0，SPI0了
  *
  * @see      IIC_InitConfig(I2C3, 400000);  //初始化IIC3  波特率400K  管脚在LQ_IIC.h中通过宏定义选择
  *
  * @date     2019/10/23 星期三
  */
void IIC_InitConfig(I2C_Type *base, uint32_t baudrate)
```

---
---
---

* 硬件IIC读写寄存器

>大部分IIC器件支持连续读取寄存器内容，具体需要自行查看器件手册。

```c

/**
  * @brief    IIC 读n个寄存器
  *
  * @param    base    ：  IIC端口号  I2C0 - I2C8
  * @param    addr    ：  器件地址（高7位） 
  * @param    reg     ：  要读的寄存器 
  * @param    val     ：  存放读出数据的地址 
  * @param    len     ：  读取长度  
  *
  * @return   读取是否成功
  *
  * @note     无
  *
  * @see      uint8_t buf;
  * @see      IIC_ReadReg(I2C3, 0x68, 0X75, &buf, 1);  //使用IIC3读取器件地址（高7位）为0x68的MPU6050  的0x75寄存器
  *
  * @date     2019/6/12 星期三
  */
status_t IIC_ReadReg(I2C_Type *base, uint16_t addr, uint8_t reg, uint8_t *val, uint8_t len) 


/**
  * @brief    IIC 写一个寄存器
  *
  * @param    base    ：  IIC端口号  I2C0 - I2C8 
  * @param    addr    ：  器件地址（高7位） 
  * @param    reg     ：  要写的寄存器 
  * @param    val     ：  要写入的值  
  *
  * @return   读取是否成功
  *
  * @note     无
  *
  * @see      IIC_WriteReg(I2C3, 0x68, 0X75, 0x10);  //使用IIC3向器件地址（高7位）为0x68的MPU6050  的0x75寄存器 写入0x10
  *
  * @date     2019/6/12 星期三
  */
status_t IIC_WriteReg(I2C_Type *base, uint16_t addr, uint8_t reg, uint8_t val) 
```

---
---
---

* 软件IIC初始化
```c
/*!
  * @brief    模拟IIC初始化
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      IIC_Init();  //模拟IIC初始化 IIC管脚在LQ_SOFTIIC.h中定义
  *
  * @date     2019/10/23 星期三
  */	
void IIC_Init(void);
```

---
---
---

* 软件IIC读写寄存器

```c
/*!
  * @brief    模拟IIC读取指定设备 指定寄存器的一个值
  *
  * @param    I2C_Addr  目标设备地址
  * @param    reg       目标寄存器
  * @param    buf       存放读出字节
  *
  * @return   1失败 0成功
  *
  * @note     无
  *
  * @see      uint8_t data;  
  * @see      IIC_ReadByteFromSlave(0xD0, 0x75, &data);   //读 IIC地址为 0xD0器件（MPU6050）寄存器0x75
  *
  * @date     2019/10/23 星期三
  */	
uint8_t IIC_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf);

/*!
  * @brief    模拟IIC读取指定设备 指定寄存器的n个值
  *
  * @param    dev       目标设备地址
  * @param    reg       目标寄存器
  * @param    length    读取长度
  * @param    data      存放读出数据
  *
  * @return   1失败 0成功
  *
  * @note     无
  *
  * @see      uint8_t data[14];  
  * @see      IIC_ReadByteFromSlave(0xD0, 0X3B, 14, &data);   //读 14个字节
  *
  * @date     2019/10/23 星期三
  */
uint8_t IIC_ReadMultByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);

/*!
  * @brief    模拟IIC写指定设备 指定寄存器的一个值
  *
  * @param    I2C_Addr  目标设备地址
  * @param    reg       目标寄存器
  * @param    data      写入的数据
  *
  * @return   1失败 0成功
  *
  * @note     无
  *
  * @see      IIC_ReadByteFromSlave(0xD0, 0X6B, 0X80);   //IIC地址为 0xD0器件（MPU6050）寄存器0x6B 写入0x80
  *
  * @date     2019/10/23 星期三
  */
uint8_t IIC_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t buf);

```

---
---
---

* IIC封装
>MPU5060-ICM20602都是IIC接口的，因此在初始化前需要先对IIC接口初始化，为了方便移植，龙邱的LQ_MPU6050.c中对IIC函数进行了一层封装，其他函数调用IIC接口都是调用封装后的接口，如果要移植，只需要修改这里即可。

龙邱的MPU6050-ICM20602底层支持模拟IIC和硬件IIC，通过config.h中的宏定义选择
```c
#if (IMU_USE_SELECT == USE_HAL_IIC)
/**
  * @brief    IIC 连续读
  *
  * @param    addr:器件地址 
  * @param    reg :要读取的寄存器地址
  * @param    len :要读取的长度
  * @param    buf :读取到的数据存储区
  *
  * @return   0 ：读取成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      uint8_t buf[14];  
  * @see      MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);  
  *
  * @date     2019/6/12 星期三
  */
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
    return IIC_ReadReg(I2C1, addr, reg, buf, len);     
}



/**
  * @brief    IIC 写一个寄存器
  *
  * @param    addr  :器件地址 
  * @param    reg   :要写的寄存器地址
  * @param    value :要写入的值
  *
  * @return   0 ：读取成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,1);	
  *
  * @date     2019/6/12 星期三
  */
uint8_t MPU_Write_Byte(uint8_t addr,uint8_t reg,uint8_t value)
{
    return IIC_WriteReg(I2C1, addr, reg, value);    
}

/**
  * @brief    IIC 读一个寄存器
  *
  * @param    addr  :器件地址 
  * @param    reg   :寄存器地址
  *
  * @return   读取的值
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);   
  *
  * @date     2019/6/12 星期三
  */
uint8_t MPU_Read_Byte(uint8_t addr,uint8_t reg)
{
    uint8_t value[1];
    IIC_ReadReg(I2C1, addr, reg, value, 1);
    return value[0];
}



#else


/**
  * @brief    IIC 连续读
  *
  * @param    addr:器件地址 
  * @param    reg :要读取的寄存器地址
  * @param    len :要读取的长度
  * @param    buf :读取到的数据存储区
  *
  * @return   0 ：读取成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      uint8_t buf[14];  
  * @see      MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2019/6/12 星期三
  */
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
    return IIC_ReadMultByteFromSlave(addr<<1, reg, len, buf);     
}



/**
  * @brief    IIC 写一个寄存器
  *
  * @param    addr  :器件地址 
  * @param    reg   :寄存器地址
  * @param    value :要写入的值
  *
  * @return   0 ：读取成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,1);	
  *
  * @date     2019/6/12 星期三
  */
uint8_t MPU_Write_Byte(uint8_t addr,uint8_t reg,uint8_t value)
{
    return IIC_WriteByteToSlave(addr<<1, reg, value);    
}


/**
  * @brief    IIC 读一个寄存器
  *
  * @param    addr  :器件地址 
  * @param    reg   :寄存器地址
  *
  * @return   读取的值
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);        
  *
  * @date     2019/6/12 星期三
  */
uint8_t MPU_Read_Byte(uint8_t addr,uint8_t reg)
{
    uint8_t value[1];
    MPU_Read_Len(addr, reg, 1, value);
    return value[0];
}


#endif
```

---
---
---

* MPU6050-ICM20602初始化

初始化设置了MPU6050的一些寄存器，用户可以自行修改。
```c
/**
  * @brief    初始化MPU6050 或者 ICM20602
  *
  * @param    无
  *
  * @return   0：初始化成功   1：失败  
  *
  * @note     使用前先初始化IIC接口
  *
  * @see      MPU6050_Init();
  *
  * @date     2019/6/12 星期三
  */
uint8_t MPU6050_Init(void)
```

* 获取原始数据

MPU6050-ICM20602是六轴传感器，因此可以读到6个原始数据。MPU6050-ICM20602的原始数据是16位带符号的在-32768 到 +32767之间。初始化时，如果加速度计量程初始化为±4G，则读取到的原始数据-32768对应-4G，原始数据0对应0G，原始数据32767对应4G，因为水平放置时Z轴受重力加速度G影响，Z轴原始数据应该在8191左右。陀螺仪原始数据对应的实际物理意义同理。（注意数据都是在一定范围内波动的，这也是为什么需要后期滤波处理）

```c
/**
  * @brief    获取 加速度值 角速度值
  *
  * @param    ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
  * @param    gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)  
  *
  * @return   0：读取成功
  *
  * @note     无
  *
  * @see      int6_t data[6];
  * @see      ICM_Get_Raw_data(&data[0], &data[1], &data[2],&data[3], &data[4], &data[5]);  
  *
  * @date     2019/6/12 星期三
  */
uint8_t MPU_Get_Raw_data(short *ax,short *ay,short *az,short *gx,short *gy,short *gz)
```

### 程序讲解

* 首先对板子时钟和debug串口初始化

```c

/* 初始化板子时钟 180M */
    BOARD_BootClockPLL180M();
    
    /* 初始化串口0 可以使用 PRINTF函数 */
    BOARD_InitDebugConsole();

    PRINTF("\r\n 北京龙邱智能科技 \r\n");
    printf("\r\n 北京龙邱智能科技 \r\n");
    
    /*设置中断优先级组  0: 0个抢占优先级 8个子优先级 
     *                  1: 2个抢占优先级 4个子优先级 
	 *		            2: 4个抢占优先级 2个子优先级 
     *                  3: 8个抢占优先级 0个子优先级 
     */
	/* 配置优先级分组 3: 8个抢占优先级 0-7  0优先级最高 */
    NVIC_SetPriorityGrouping(NVIC_Group3);
```

---
---
---

* IIC接口初始化（因为摄像头会影响硬件IIC时序，所以这里使用模拟IIC），屏幕串口初始化，MPU6050初始化

>OLED和TFT18都可以使用，具体使用那个屏显示，请打开config.h，选择对应的显示屏。

```c
    char  txt[30];
	short aacx,aacy,aacz;	         //加速度传感器原始数据
	short gyrox,gyroy,gyroz;        //陀螺仪原始数据
    LCD_Init();                      //LCD初始化
    LCD_CLS();                       //LCD清屏
    
#if (IMU_USE_SELECT == USE_HAL_IIC)
    IIC_InitConfig(I2C1, 400000);
#else
    IIC_Init(); 
#endif
    
    LCD_P8x16Str(15,0,"LQ 6050 Test"); 
    PRINTF("\r\nLQ 6050 Test");
    
    if(MPU6050_Init())
    {
    
        LCD_P8x16Str(15,2,"6050 Test Fail");
        while(1);
    }
```

* 在循环中，不断读取显示MPU6050原始数据

```c
while(1)
    {
        MPU_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);	//得到加速度传感器数据  
        sprintf((char*)txt,"ax:%06d",aacx);
        LCD_P6x8Str(0,2,txt);
        sprintf((char*)txt,"ay:%06d",aacy);
        LCD_P6x8Str(0,3,txt);
        sprintf((char*)txt,"az:%06d",aacz);
        LCD_P6x8Str(0,4,txt);
        sprintf((char*)txt,"gx:%06d",gyrox);
        LCD_P6x8Str(0,5,txt);
        sprintf((char*)txt,"gy:%06d",gyroy);
        LCD_P6x8Str(0,6,txt);
        sprintf((char*)txt,"gz:%06d",gyroz);
        LCD_P6x8Str(0,7,txt);

        PRINTF("\r\nAX: %d  ",aacx); 
        PRINTF("\r\nAY: %d  ",aacy);
        PRINTF("\r\nAZ: %d  ",aacz); 
        PRINTF("\r\nGX: %d  ",gyrox);
        PRINTF("\r\nGY: %d  ",gyroy); 
        PRINTF("\r\nGZ: %d  ",gyroz);
        delayms(100);
        PRINTF("\r\n*********************\r\n");
    }
```

---
---
---

### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个；
2) LPC54606 迷你 子母板一套+TFT18/OLED一块+MPU6050/ICM20602；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
2) 打开配套程序，连接下载器；
3) 插上TFT18/OLED，接好MPU6050/ICM20602；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
屏幕上会显示MPU6050/ICM20602原始数据。

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 

注意，IIC接线一定要结实稳定。