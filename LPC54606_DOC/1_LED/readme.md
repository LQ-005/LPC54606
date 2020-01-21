
[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# LED例程

> 点亮小灯，迈向嵌入式世界的第一步。

# 目录  
- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [GPIO简介](###GPIO简介)  
  - [常用API](###常用API) 
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项) 


## 硬件电路

龙邱LPC54606板子上有一个RGB七彩灯，打开资料中的原理图，接线如下    

      LED_R红灯       P0_8                                                         
      LED_G绿灯       P0_9                                                         
      LED_B蓝灯       P1_12 

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB327d8af2ac42ce8c615cfad854b9646e?method=download&shareKey=2e8fdef8779892996227be6a52bb7509">
</div>

RGB灯旁边是电源灯，如果芯片正常，给核心板供电，则电源灯就会常亮；如果供电正，电源灯却不亮，断电用万用表测量核心板VCC和GND之间的电阻，小于1K则极大可能芯片烧坏，可以联系淘宝客服寄回维修。

## 软件实现

>这是一个简易的教程，只是为了让大家快速入门，熟悉相应的API，调用对应API实现自己的功能即可。如果不是做嵌入式底层的，没有太大必要去深究寄存器，毕竟现在嵌入式的趋势是将底层和上层分开，减少用户的开发周期。

### GPIO简介

GPIO 是通用输入输出端口的简称，简单来说就是 LPC54606 可控制的引脚。LPC54606J512一共有两组GPIO，每组32个IO。
```c
    typedef enum  
    {
        P0_0=0,  P0_1,  P0_2,  P0_3,  P0_4,  P0_5,  P0_6,  P0_7,
        P0_8,    P0_9,  P0_10, P0_11, P0_12, P0_13, P0_14, P0_15,
        P0_16,   P0_17, P0_18, P0_19, P0_20, P0_21, P0_22, P0_23,
        P0_24,   P0_25, P0_26, P0_27, P0_28, P0_29, P0_30, P0_31,
                                                            
        P1_0,    P1_1,  P1_2,  P1_3,  P1_4,  P1_5,  P1_6,  P1_7,
        P1_8,    P1_9,  P1_10, P1_11, P1_12, P1_13, P1_14, P1_15,
        P1_16,   P1_17, P1_18, P1_19, P1_20, P1_21, P1_22, P1_23,
        P1_24,   P1_25, P1_26, P1_27, P1_28, P1_29, P1_30, P1_31,
    }GPIO_Name_t;
```
### 常用API

>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>


* GPIO初始化
>LPC每个管脚都有多种功能，可以作为GPIO也可以复用为其他功能，怎么决定管脚用作什么功能呢？在LPC中每一个管脚都有一个IOCON寄存器，用来控制管脚的复用功能和一些电气特性。

GPIO初始化函数主要的参数时mode，GPIO输出模式时，比较常用的是推挽输出模式（可以高速切换管脚输出高低电平）；另一种常用的输出模式是开漏输出（只可以输出低电平，想要输出高电平需要外部上拉，上拉电阻越大，输出高电平上升越慢，做不同器件电平匹配时可以使用开漏输出）。在LQ_GPIO.h中，已经将GPIO常用的模式通过宏定义方式写出，使用PIN_InitConfig()函数时，mode参数直接从宏定义选择即可，也可以自行配置。
```c

/* GPIO常用配置 */
#define PIN_MODE_OUTPUT         OUTPUT|FUNC0|MODE_Diaable|MODE_GPIO|INVERT_Disable|DIGIMODE_Digital|FILTER_Disable|SLEW_Standard|OD_Disable   /*!< 推挽输出  */
#define PIN_MODE_INPUT          INPUT|FUNC0|MODE_Diaable|MODE_GPIO|INVERT_Disable|DIGIMODE_Digital|FILTER_Disable|SLEW_Standard|OD_Enable     /*!< 浮空输入  */
#define PIN_MODE_INPUT_PULLUP   INPUT|FUNC0|MODE_Up|MODE_GPIO|INVERT_Disable|DIGIMODE_Digital|FILTER_Disable|SLEW_Standard|OD_Disable         /*!< 上拉输入  */
#define PIN_MODE_INPUT_PULLDOWN INPUT|FUNC0|MODE_Down|MODE_GPIO|INVERT_Disable|DIGIMODE_Digital|FILTER_Disable|SLEW_Standard|OD_Disable       /*!< 下拉输入  */
#define PIN_MODE_OUTPUT_OD      OUTPUT|FUNC0|MODE_Diaable|MODE_GPIO|INVERT_Disable|DIGIMODE_Digital|FILTER_Disable|SLEW_Standard|OD_Enable    /*!< 开漏输出  */


       /*!
        * @brief    GPIO初始化
        *
        * @param    pin      ： 管脚标号  LQ_GPIO.h中定义
        * @param    mode     ： GPIO 模式   LQ_GPIO.h中的宏定义中选择
        * @param    output   ： 输出模式时，输出电平 1：高电平  0：低电平   
        *
        * @return   无
        *
        * @note     无
        *
        * @see      PIN_InitConfig(P0_0, PIN_MODE_OUTPUT, 1);  //P0_0初始化推挽输出 高电平
        *
        * @date     2019/10/18 星期五
        */
        void PIN_InitConfig(GPIO_Name_t pin, uint32_t mode, uint8_t output);
```
---
---
---

* GPIO操作
>常用GPIO操作，写、改变输入输出模式、电平翻转。
```c
    /*!
    * @brief    设置GPIO输出状态
    *
    * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
    * @param    output   ： 1:高电平  0：低电平
    *
    * @return   无
    *
    * @note     使用前先初始化IO
    *
    * @see      PIN_Write(P0_0, 1);//P0_0置为高电平
    *
    * @date     2019/8/19 星期一
    */
    void PIN_Write(GPIO_Name_t pin, uint8_t output);



    /*!
    * @brief    设置GPIO输入输出
    *
    * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
    * @param    mode     ： 1:输出  0：输入
    *
    * @return   无
    *
    * @note     使用前请先初始化
    *
    * @see      PIN_Dir(P0_0, 1);//P0_0设置为输出
    *
    * @date     2019/8/19 星期一
    */
    void PIN_Dir(GPIO_Name_t pin, uint8_t mode);



    /*!
    * @brief    GPIO翻转函数
    *
    * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
    *
    * @return   无
    *
    * @note     使用前请先初始化
    *
    * @see      PIN_Reverse(P0_0); //翻转P0_0管脚
    *
    * @date     2019/10/18 星期五
    */
    void PIN_Reverse(GPIO_Name_t pin);
```

* 51风格的IO操作

> 为了方便使用，使用宏定义对GPIO的寄存器进行封装，在GPIO初始化后，可以使用对应宏定义对IO进行操作。
```c
/*! 定义PT0_的端口  类似51风格的操作 */
#define PT0_0     GPIO->B[0][0]
#define PT0_1     GPIO->B[0][1]
#define PT0_2     GPIO->B[0][2]
#define PT0_3     GPIO->B[0][3]
...
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

* 初始化LED相连的IO为输出模式

```c

/**
  * @brief    RGB灯引脚初始化
  *
  * @param    无
  *
  * @return   无
  *
  * @note     LED使用IO在LQ_LED.h中通过宏定义选择
  *
  * @see      LED_Init();  
  *
  * @date     2019/6/4 星期二
  */
void LED_Init(void)
{
    PIN_InitConfig(LED_R_PIN,  PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(LED_G_PIN,  PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(LED_B_PIN,  PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(LED_D0_PIN, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(LED_D1_PIN, PIN_MODE_OUTPUT, 0);
    
}

```

---
---
---
* 在循环中，指定不同RGB灯亮

```c
/**
  * @brief    指定颜色的灯亮
  *
  * @param    color  ： 枚举LED_t中的颜色
  *
  * @return   无
  *
  * @note     使用前请先初始化
  *
  * @see      LED_Color(red);   //红灯亮
  *
  * @date     2019/6/4 星期二
  */
void LED_Color(LED_t color)
{
    switch(color)
    {
        case white:
            PIN_Write(LED_G_PIN,  0U);//LED亮; 
            PIN_Write(LED_R_PIN,  0U);//LED亮; 
            PIN_Write(LED_B_PIN,  0U);//LED亮; 
            break;
        case black:
            PIN_Write(LED_G_PIN,  1U);//LED灭; 
            PIN_Write(LED_R_PIN,  1U);//LED灭; 
            PIN_Write(LED_B_PIN,  1U);//LED灭; 
            break;
        case red:
            PIN_Write(LED_G_PIN,  1U);//LED灭; 
            PIN_Write(LED_R_PIN,  0U);//LED亮; 
            PIN_Write(LED_B_PIN,  1U);//LED灭; 
            break;
        case green:
            PIN_Write(LED_G_PIN,  0U);//LED亮; 
            PIN_Write(LED_R_PIN,  1U);//LED灭; 
            PIN_Write(LED_B_PIN,  1U);//LED灭; 
            break;
        case blue:
            PIN_Write(LED_G_PIN,  1U);//LED灭; 
            PIN_Write(LED_R_PIN,  1U);//LED灭; 
            PIN_Write(LED_B_PIN,  0U);//LED亮; 
            break;
        case yellow:
            PIN_Write(LED_G_PIN,  0U);//LED亮; 
            PIN_Write(LED_R_PIN,  0U);//LED亮; 
            PIN_Write(LED_B_PIN,  1U);//LED灭; 
            break;
        case violet:
            PIN_Write(LED_G_PIN,  1U);//LED灭; 
            PIN_Write(LED_R_PIN,  0U);//LED亮; 
            PIN_Write(LED_B_PIN,  0U);//LED亮; 
            break;
        case cyan:
            PIN_Write(LED_G_PIN,  0U);//LED亮; 
            PIN_Write(LED_R_PIN,  1U);//LED灭; 
            PIN_Write(LED_B_PIN,  0U);//LED亮; 
            break;      
    }
}


/**
  * @brief    测试LED灯
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  * 
  * @see  
  *
  * @date     2019/6/4 星期二
  */
void Test_LED(void)
{
  LED_Init();   
  while (1)
  {          
        LED_Color(red);     //红灯
        delayms(500);
        LED_Color(green);   //绿灯
        delayms(500);
        LED_Color(blue);    //蓝灯
        delayms(500);
        LED_Color(violet);  //紫灯
        delayms(500);
        LED_Color(yellow);  //黄灯
        delayms(500);
        LED_Color(cyan);    //青灯
        delayms(500);
        LED_Color(white);   //白灯
        delayms(500);
        LED_Color(black);   //关灯
        delayms(500);
        
        for(int i = 0; i < 20; i++)
        {
            LED_ColorReverse(red);     //红灯闪烁
            delayms(100);
        }
        for(int i = 0; i < 20; i++)
        {
            LED_ColorReverse(green);   //绿灯闪烁
            delayms(100);
        }
        for(int i = 0; i < 20; i++)
        {
            LED_ColorReverse(blue);    //蓝灯闪烁
            delayms(100);
        }
        for(int i = 0; i < 20; i++)
        {
            LED_ColorReverse(violet);  //紫灯闪烁
            delayms(100);
        }
        for(int i = 0; i < 20; i++)
        {
            LED_ColorReverse(yellow);  //黄灯闪烁
            delayms(100);
        }
        for(int i = 0; i < 20; i++)
        {
            LED_ColorReverse(cyan);    //青灯闪烁
            delayms(100);
        }
        for(int i = 0; i < 20; i++)
        {
            LED_ColorReverse(white);   //白灯闪烁
            delayms(100);
        }
        
        for(int i = 0; i < 20; i++)
        {
            PIN_Write(LED_D0_PIN,  1U);//LED亮;
            PIN_Write(LED_D1_PIN,  1U);//LED亮;
            delayms(100);
            PIN_Write(LED_D0_PIN,  0U);//LED亮;
            PIN_Write(LED_D1_PIN,  0U);//LED亮;
            delayms(100);
        }

  }
}
```

### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个（jlink V9 也可以）；
2) LPC54606 迷你 核心板一块；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
3) 打开配套程序，连接下载器；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
核心板上的RGB灯闪烁

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 
