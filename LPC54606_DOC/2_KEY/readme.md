[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# 按键例程

> 点亮小灯，是GPIO输出功能，按键则是输入。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [GPIO简介](###GPIO简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)


## 硬件电路

龙邱LPC54606母板上有三个按键，打开资料中的原理图，接线如下    

      KEY0_PIN       P0_21
                    
      KEY1_PIN       P0_5 
                    
      KEY2_PIN       P1_14


<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB735358ade9a9cf7d2e06d849ad6c0407?method=download&shareKey=7725b54fa4fa0a4b707a8371a776c837">
</div>

与按键相连的IO通过4.7K电阻上拉到3.3V，当按键按下，与按键相连的IO就会读到低电平，否则是高电平(*注意，这里GPIO一定要设置为输入模式，如果设置为输出高电平，当按键按下，高低电平直接短接可能会损坏芯片*)。


## 软件实现

>这是一个简易的教程，只是为了让大家快速入门，熟悉相应的API，调用对应API实现自己的功能即可。如果不是做嵌入式底层的，没有太大必要去深究寄存器，毕竟现在嵌入式的趋势是将底层和上层分开，减少用户的开发周期。

### GPIO简介

GPIO 是通用输入输出端口的简称，简单来说就是 LPC54606 可控制的引脚。LPC54606J512一共有两组GPIO，每组32个IO。

### 常用API

>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>


* GPIO初始化
>LPC每个管脚都有多种功能，可以作为GPIO也可以复用为其他功能，LED例程中我们使用了GPIO的输出模式，本例程则使用输入模式，读取外部电平状态。

GPIO初始化函数主要的参数时mode，GPIO输入模式时，可以选择合适的上下拉电阻。比如按键电路，母板上可以去掉外部的4.7K上拉电阻，使用内部上拉的方式。在LQ_GPIO.h中，已经将GPIO常用的模式通过宏定义方式写出，使用PIN_InitConfig()函数时，mode参数直接从宏定义选择即可，也可以自行配置。
```c
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
>常用GPIO操作，读去IO高低电平。

```c
    /*!
    * @brief    GPIO初始化
    *
    * @param    pin      ： 管脚标号  LQ_GPIO.h中定义  
    *
    * @return   读取的电平  1：高电平  0：低电平   
    *
    * @note     使用前先初始化IO
    *
    * @see      PIN_Read(P0_0, PIN_MODE_OUTPUT, 1);  //P0_0初始化推挽输出 高电平
    *
    * @date     2019/10/18 星期五
    */
    uint8_t PIN_Read(GPIO_Name_t pin);

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

* 初始化按键相连的IO为输入模式，并且初始化LED灯

```c
/**
  * @brief    初始化龙邱母板上的按键
  *
  * @param    无
  *
  * @return   无
  *
  * @note     按键IO在LQ_KEY.h中通过宏定义选择
  *
  * @see      KEY_Init();
  *
  * @date     2019/6/4 星期二
  */
void KEY_Init(void)
{  
    /* 设置上拉输入 */
    PIN_InitConfig(KEY0_PIN,  PIN_MODE_INPUT_PULLUP, 1);
    PIN_InitConfig(KEY1_PIN,  PIN_MODE_INPUT_PULLUP, 1);
    PIN_InitConfig(KEY2_PIN,  PIN_MODE_INPUT_PULLUP, 1);  
}

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
* 在循环中，不断读取按键状态，通过按键状态指定不同RGB灯亮

```c
/**
  * @brief    读取按键状态
  *
  * @param    mode： 0：不支持连续按;   1：支持连续按;
  *
  * @return   0：无按键按下  1：KEY0按下  2:KEY1按下  3:KEY2按下
  *
  * @note     使用前请先初始化按键
  *
  * @see      uint8_t key_flag = KEY_Read(0);
  *
  * @date     2019/6/4 星期二
  */
uint8_t KEY_Read(uint8_t mode)
{
    static uint8_t key_up=1;     //按键松开标志
    if(mode==1) 
    {
        key_up=1;      //支持连按
    }
    if(key_up && (PIN_Read(KEY0_PIN)==0 || PIN_Read(KEY1_PIN)==0 || PIN_Read(KEY2_PIN)==0))
    {
        delayms(10);   //消抖
        key_up=0;
        if(PIN_Read(KEY0_PIN)==0)      
        {
            return 1;
        }
        
        else if(PIN_Read(KEY1_PIN)==0) 
        {
            return 2;    
        }
        
        else if(PIN_Read(KEY2_PIN)==0) 
        {
            return 3;    
        }
        
    }
    if(PIN_Read(KEY0_PIN)==1 && PIN_Read(KEY1_PIN)==1 && PIN_Read(KEY2_PIN)==1) 
    {
        key_up=1;   
    }
    return 0;   //无按键按下
    
}
/**
  * @brief    测试按键
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      Test_KEY();
  *
  * @date     2019/6/4 星期二
  */
void Test_KEY(void)
{ 
    LED_Init();
    KEY_Init(); 
    while (1)
    {  
        switch(KEY_Read(1))  
        {
            case 1:
                LED_Color(red);     
                break;           
            case 2:      
                LED_Color(green);   
                break;
            case 3:      
                LED_Color(blue);    
                break;
            default:
                LED_Color(white);   
                break;
        }

        delayms(50);
    }
}
```



### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个（jlink V9 也可以）；
2) LPC54606 迷你 子母板一套；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
3) 打开配套程序，连接下载器；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
按下母板上的按键，核心板上对应的RGB灯闪烁。

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 
