[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# VL53例程

>VL53激光测距模块测试例程。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [IIC简介](###IIC简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)

## 硬件电路

VL53也是IIC接口的，因此也可以和母板上的陀螺仪接口、摄像头的SCCB接口公用一个IIC总线（摄像头只是初始化时占用，初始化完成后就不会使用了，因此如果使用摄像头需要先初始化摄像头）
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

速度上，对于最大通信速度400K的VL53来说，软硬件IIC速度没有差别，不过模拟IIC想要调节速率需要借助示波器工具。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB76389fd37de7da0224ef48ac250884cf?method=download&shareKey=fec1a9e02f17a9bf408d814042bedc2c">
</div>

硬件IIC地址用的是7位地址，龙邱的模拟IIC地址用的是写地址，即硬件IIC地址乘以2（VL53使用硬件IIC时地址传入0x29，使用龙邱模拟IIC时传入地址0x52）。

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
>为了方便移植，龙邱的TEST_VL53.c中对IIC函数进行了一层封装，其他函数调用IIC接口都是调用封装后的接口，如果要移植，只需要修改这里即可。

龙邱的VL53底层支持模拟IIC和硬件IIC，通过config.h中的宏定义选择
```c

#if (VL53_USE_SELECT == USE_HAL_IIC)

/**
  * @brief    VL53 写1个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    data：   写入数据
  *
  * @return   
  *
  * @note     
  *
  * @see  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Write_Byte(uint8_t dev, uint8_t reg, uint8_t data)
{
    IIC_WriteReg(I2C1, dev, reg, data);    
}

/**
  * @brief    VL53 读n个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    length;  长度
  * @param    data：   指向存放读数据
  *
  * @return   
  *
  * @note     
  *
  * @see  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Read_nByte(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
    IIC_ReadReg(I2C1, dev, reg, data, length);   
}


#else

/**
  * @brief    VL53 写1个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    data：   写入数据
  *
  * @return   
  *
  * @note     
  *
  * @see  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Write_Byte(uint8_t dev, uint8_t reg, uint8_t data)
{
    IIC_WriteByteToSlave(dev<<1, reg, data);  
}

/**
  * @brief    VL53 读n个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    length;  长度
  * @param    data：   指向存放读数据
  *
  * @return   
  *
  * @note     
  *
  * @see  
  *
  * @date     2019/4/29 星期一
  */
void VL53_Read_nByte(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
{
    IIC_ReadMultByteFromSlave(dev<<1, reg, length, data);    
}

#endif 
```

---
---
---

* VL53修改IIC地址

和其他IIC器件相比，VL53支持自定义IIC地址，每次上电后VL53模块地址为0x29,可以通过修改VL53地址寄存器，修改VL53IIC地址（掉电后丢失），这样可以方便一个IIC总线上挂载多个VL53模块。

```c
/* 修改IIC地址后，将VL53地址改为 VL53NEWADDR */
    VL53_Write_Byte(VL53ADDR, VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS, VL53NEWADDR);
```

* 开始测距

VL53官方推荐的是库开发，但是库非常繁杂，为了简化使用，提取出常用的寄存器，进行寄存器操作。

```c
uint8_t VL53_STAR = 0x02;    //0x02 连续测量模式    0x01 单次测量模式

/* 开始测距 */
VL53_Write_Byte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, VL53_STAR);
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

* IIC接口初始化（因为摄像头会影响硬件IIC时序，所以这里使用模拟IIC），屏幕串口初始化，VL53初始化

>OLED和TFT18都可以使用，具体使用那个屏显示，请打开config.h，选择对应的显示屏。

```c
    LED_Init();
	PRINTF("\r\nLQ VL53 Test");
	
#ifdef OLED
	LCD_Init();
    LCD_CLS();
	LCD_P8x16Str(5,0,"LQ VL53 Test");
#else
	TFTSPI_Init(1);                //TFT1.8初始化  0：横屏显示  1：竖屏显示  
    TFTSPI_CLS(u16BLUE);           //清屏
	TFTSPI_P8X16Str(0,0,"LQ VL53 Test",u16RED,u16BLUE);
#endif
	char txt[16];
	
#if (VL53_USE_SELECT == USE_HAL_IIC)
    IIC_InitConfig(I2C1, 400000);
#else
    IIC_Init(); 
#endif       

    uint8_t VL53_STAR = 0x02;    //0x02 连续测量模式    0x01 单次测量模式
    uint8_t dis_buff[2];
    uint16_t dis, last_dis;
	/* 开始测距 */
    VL53_Write_Byte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, VL53_STAR);
```

* 在循环中，不断读取显示VL53数据，注意VL53测距频率最大30Hz

```c
while(1)
    {
        /* 获取测量数据 */
		VL53_Read_nByte(VL53ADDR, VL53_REG_DIS, 2, dis_buff);
    
        /* 转换数据 */
        dis = (dis_buff[0]<<8) | (dis_buff[1]);
        
        /* 超过2M没有障碍 */
        if(dis > 8000)
        {
            dis = 0;
        }
     
        if(dis == 20)
        {
            dis = last_dis;
        }
        last_dis = dis;
		sprintf(txt, "DIS %5d mm",dis);
#ifdef OLED
		LCD_P8x16Str(0,5,txt);
#else
		TFTSPI_P8X16Str(2,5,txt,u16RED,u16BLUE);
#endif
		delayms(50);
		LED_ColorReverse(blue);
    
    }
```

---
---
---

### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个；
2) LPC54606 迷你 子母板一套+TFT18/OLED一块+VL53一个；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
2) 打开配套程序，连接下载器；
3) 插上TFT18/OLED，接好VL53；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
屏幕上会显示距离信息。

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 

注意，IIC接线一定要结实稳定。