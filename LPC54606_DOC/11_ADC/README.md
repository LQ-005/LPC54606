[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# ADC例程

>ADC是一种比较常用的功能，可用于测量电压。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [ADC简介](###ADC简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)


## 硬件电路

>LPC54606最多8个ADC通道，其中两个还是下载口，资源比较有限。母板上引出了6个ADC接口，但是需要注意P0_15还被编码器4接口使用，因此编码器4和ADC通道P0_15只能同时使用其中一个。


<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBf121cd6d3629d6261ca8c186c5f241ab?method=download&shareKey=04a806d24c6ce376557de43d443d43ef">
</div>

## 软件实现

### ADC简介

> ADC 即模拟数字转换器（英语： Analog-to-digital converter）是用于将模拟形式的连续信号转换为数字形式的离散信号的一类设备。简单点来说就是测量电压的，一些模拟类的传感器，将测量值转化为电压值，单片机使用ADC读取电压值获取传感器信息。对于智能车用户来讲，一般有两个用途，测量电池电压，防止电池过放；测量电磁传感器经运放放大后的电压的信号（注意先调节好运放的放大倍数，确定放大后的信号不会超过 3.3V，再接到单片机的 AD 口）。

### 常用API

>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>

* ADC初始化

ADC初始化函数需要两个参数，第一个是ADC通道，是一个LQ_ADC.h中的枚举体。第二个参数是当前通道的ADC精度，也是LQ_ADC.h中的一个枚举体。ADC精度可选8位、10位、12位。

```c

/** 
  * @brief ADC通道
  */ 
typedef enum  //ADC模块
{
    //ADC通道 管脚关系        LQ_546XXP100SYS       
    ADC0CH0_P0_10=0,     /*!< P0_10    可用                  */              
    ADC0CH1_P0_11=1,     /*!< P0_11    可用（SWD下载器口）   */
    ADC0CH2_P0_12=2,     /*!< P0_12    可用（SWD下载器口）   */
    ADC0CH3_P0_15=3,     /*!< P0_15    可用                  */
    ADC0CH4_P0_16=4,     /*!< P0_16    可用                  */
    ADC0CH5_P0_31=5,     /*!< P0_31    可用                  */
    ADC0CH6_P1_0=6,      /*!< P1_0     可用                  */     
    ADC0CH11_P0_23=11,   /*!< P0_23    可用                  */ 
} ADCn_Ch;


/** 
  * @brief ADC精度
  */
typedef enum ADC_nbit
{
    ADC_8bit   = 0x01,    /*!< 分辨率 8位                  */       
    ADC_10bit  = 0x02,    /*!< 分辨率 10位                 */     
    ADC_12bit  = 0x03,    /*!< 分辨率 16位                 */     
} ADC_nbit;

/*!
  * @brief    ADC通道初始化
  *
  * @param    channel   ：  ADC通道 LQ_ADC.h中的一个枚举体  
  * @param    bit       ：  ADC通道精度 LQ_ADC.h中的一个枚举体  
  *
  * @return   无
  *
  * @note     读取ADC之前一定要调用该函数对ADC通道进行初始化
  *
  * @see      ADC_InitConfig(ADC0CH0_P0_10, ADC_12bit);  //初始化ADC通道0 P0_10
  *
  * @date     2019/10/21 星期一
  */
void ADC_InitConfig(ADCn_Ch channel, ADC_nbit bit)
```

---
---
---

* 读取ADC值

>读取ADC的值和ADC初始化时通道精度有关，如果通道精度12位，则读出ADC数据0代表0V，4095(2^12-1) 代表3.3V。

```c

/**
  * @brief    获取ADC的测量值
  *
  * @param    channel ：adc通道
  *
  * @return   测量值
  *
  * @note     使用前需要初始化ADC通道
  *
  * @see      ADC_Read(ADC0CH0_P0_10); //读取一次ADC值
  *
  * @date     2019/5/7 星期二
  */
uint16_t ADC_Read(ADCn_Ch channel);


/*!
  * @brief    ADC读取多次 返回平均值
  *
  * @param    channel ：adc通道
  * @param    count   ：ADC读取次数
  *
  * @return   多次读取的平均值
  *
  * @note     使用前需要初始化ADC通道
  *
  * @see      ADC_ReadMean(ADC0CH0_P0_10, 10);  //读取P0_10 十次返回平均值
  *
  * @date     2019/9/30 星期一
  */
uint16_t ADC_ReadMean(ADCn_Ch channel, uint8_t count);
```

---
---
---

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

* ADC通道，初始化按键，初始化串口

>OLED和TFT18都可以使用，具体使用那个屏显示，请打开config.h，选择对应的显示屏。

```c
/* 初始化串口0 */
    UART_InitConfig(USART0, 115200);
    LCD_Init();                  //LCD初始化
    LCD_CLS();                   //LCD清屏
    LED_Init();
    KEY_Init();
    
    ADC_InitConfig(ADC0CH0_P0_10, ADC_12bit);
    ADC_InitConfig(ADC0CH3_P0_15, ADC_12bit);
    ADC_InitConfig(ADC0CH4_P0_16, ADC_12bit);   //注意会占用编码器4接口
    ADC_InitConfig(ADC0CH5_P0_31, ADC_12bit);
    ADC_InitConfig(ADC0CH6_P1_0,  ADC_12bit);
    ADC_InitConfig(ADC0CH11_P0_23, ADC_12bit);

    LCD_P8x16Str(4,0,"LQ ADC Test"); 
    
    UART_PutStr(USART0, "\r\nLQ ADC Test");
```

* 由于OLED一次显示不完，因此使用按键，记录标志位，根据标志位显示不同的页面。

```c
/* 0.806 = 3.3 / 4095 * 1000    12位AD 4096*/
    uint16_t batv1=(uint16_t)(ADC_Read(ADC0CH4_P0_16)*0.806);  //获取ADC值 
    uint16_t batv2=(uint16_t)(ADC_Read(ADC0CH6_P1_0)*0.806);  //获取ADC值 
    uint16_t batv3=(uint16_t)(ADC_Read(ADC0CH3_P0_15)*0.806);  //获取ADC值
    uint16_t batv4=(uint16_t)(ADC_Read(ADC0CH0_P0_10)*0.806);  //获取ADC值 
    uint16_t batv5=(uint16_t)(ADC_Read(ADC0CH5_P0_31)*0.806);  //获取ADC值 
    uint16_t batv6=(uint16_t)(ADC_Read(ADC0CH11_P0_23)*0.806);  //获取ADC值 
    
    char txt[32];
    uint8_t key = 0;
    while (1)
    {      
		
        /* 这里乘以0.5 做一个简单的低通滤波 防止数据跳动太快 屏幕看不清楚 */
        batv1 = (uint16_t)(batv1*0.5 + 0.5*ADC_Read(ADC0CH4_P0_16)*0.806);            
        batv2 = (uint16_t)(batv2*0.5 + 0.5*ADC_Read(ADC0CH6_P1_0)*0.806);            
        batv3 = (uint16_t)(batv3*0.5 + 0.5*ADC_Read(ADC0CH3_P0_15)*0.806);            
        batv4 = (uint16_t)(batv4*0.5 + 0.5*ADC_Read(ADC0CH0_P0_10)*0.806);            
        batv5 = (uint16_t)(batv5*0.5 + 0.5*ADC_Read(ADC0CH5_P0_31)*0.806);            
        batv6 = (uint16_t)(batv6*0.5 + 0.5*ADC_Read(ADC0CH11_P0_23)*0.806); 
        
           
        switch(KEY_Read(1))     //按键按下翻页
        {
            case 1:
                LCD_CLS();          //LCD清屏
                key = 0;            //K0 按下 第一页
                break;           
            case 2: 
                LCD_CLS();          //LCD清屏
                key = 1;            //K1 按下 第二页
                break;
            default:
                
                break;
        }
        UART_PutStr(USART0, "\r\n-----------------------------\n");
        
        if(0 == key)
        {
            LCD_P8x16Str(4,0,"LQ ADC Test"); 
            
            sprintf(txt,"P0_16:%3.2fV ",batv1/1000.0f);
            UART_PutStr(USART0, txt);
            LCD_P8x16Str(20,2,txt);
            
            sprintf(txt,"P1_0 :%3.2fV ",batv2/1000.0f);
            UART_PutStr(USART0, txt);
            LCD_P8x16Str(20,4,txt);
            
            sprintf(txt,"P0_15:%3.2fV ",batv3/1000.0f);
            UART_PutStr(USART0, txt);
            LCD_P8x16Str(20,6,txt);
            
        }
        if(1 == key)
        {            
            sprintf(txt,"P0_10:%3.2fV ",batv4/1000.0f);
            UART_PutStr(USART0, txt);
            LCD_P8x16Str(20,0,txt);
            
            sprintf(txt,"P0_31:%3.2fV ",batv5/1000.0f);
            UART_PutStr(USART0, txt);
            LCD_P8x16Str(20,2,txt);    
            
            sprintf(txt,"P0_23:%3.2fV ",batv6/1000.0f);
            UART_PutStr(USART0, txt);
            LCD_P8x16Str(20,4,txt);
            
        }
   
        LED_ColorReverse(blue);    //蓝灯   
        delayms(100);
        
    }
```

---
---
---

### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个；
2) LPC54606 迷你 子母板一套+TFT18/OLED一块；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
2) 打开配套程序，连接下载器；
3) 插上TFT18/OLED；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
屏幕上会显示ADC测量值。

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 

ADC电压测量电压不要超过3.3V。