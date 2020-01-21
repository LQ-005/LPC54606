[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# SYSTICK例程

> Systick是属于Cortex-M内核的定时器，Cortex-M内核的单片机都有这个定时器，使用和移植都十分方便。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [Systick简介](###Systick简介)  
  - [常用API](###常用API)
  - [函数指针](###函数指针)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)


## 硬件电路

>>systick是Cortex-M内核外设，Cortex-M内核的单片机都有这个定时器，因此操作系统的时钟节拍通常使用该定时器，降低移植难度。

## 软件实现

>>这是一个简易的教程，只是为了让大家快速入门，熟悉相应的API，调用对应API实现自己的功能即可。如果不是做嵌入式底层的，没有太大必要去深究寄存器，毕竟现在嵌入式的趋势是将底层和上层分开，减少用户的开发周期。

### Systick简介

systick是一个24位的递减计数器，支持中断功能，使用系统时钟（默认180Mhz）

### 常用API

>>>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>


* Systick初始化

systick是一个24位的向下计数的计数器，当递减到0时触发中断，重新填充计数值重新开启向下计数。systick初始化其实就是填充计数初始值，确定中断周期（通过宏定义 EACH_PER_MS 确定中断周期）。注意一般情况下为了系统实时性中断服务函数是尽可能短的，但是一些同学喜欢在中断服务函数中做处理，而systick定时器默认中断优先级是最低的，不会打断其他中断服务函数，因此如果想要在中断服务函数中使用systick，需要提高systick的中断优先级，可以自行修改。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB57a95b05af7baf5f136912dc3d361638?method=download&shareKey=1b788305f503e7998144d739af2bcbd3">
</div>

```c
/*!
  * @brief    systime 初始化
  *
  * @param    无
  *
  * @return   无
  *
  * @note     默认优先级最低 可以修改
  *
  * @see      systime.init();  //初始化  
  *
  * @date     2019/6/10 星期一
  */
void systime_init(void)
```

---
---
---

* 获取当前时间
>>>初始化systick后，systick就会不断递减，递减到0会进入中断服务函数，在中断服务函数中有一个变量记录ms值。获取当前时间，其实就是获取ms值。注意记录ms值的变量是一个uint32_t类型的变量，时间太长该变量会溢出（一般情况下不会有影响）

```c
/*!
  * @brief    获取当前时间
  *
  * @param    无
  *
  * @return   当前ms值
  *
  * @note     无
  *
  * @see      system.get_time_ms();  //获取当前ms值  
  *
  * @date     2019/6/10 星期一
  */
static inline uint32_t systime_get_current_time_ms(void)

/*!
  * @brief    获取当前时间
  *
  * @param    无
  *
  * @return   当前us值
  *
  * @note     无
  *
  * @see      system.get_time_us();  //获取当前ms值  
  *
  * @date     2019/6/10 星期一
  */
static inline uint32_t systime_get_current_time_us(void)
```

---
---
---

* 延时功能

>>>延时功能就是获取当前时间，然后一直等待当前时间到达设定。

```c
/*!
  * @brief    systime 延时函数
  *
  * @param    us    ：延时时间
  *
  * @return   无
  *
  * @note     最大延时不要超过 4292s
  *
  * @see      system.delay_us(1000);  //延时1ms
  *
  * @date     2019/6/10 星期一
  */
void systime_delay_us(uint32_t us)     
{
    uint32_t now = systime.get_time_us();
	uint32_t end_time = now + us - 3;
	while( systime.get_time_us() <= end_time)
    {
        ;
    }
}



/*!
  * @brief    延时函数
  *
  * @param    ms :  延时时间
  *
  * @return   无
  *
  * @note     无 
  *
  * @see      system.delay_ms(1000);  //延时1s  
  *
  * @date     2019/6/10 星期一
  */
void systime_delay_ms(uint32_t ms) 
{
    while(ms--)
    {
        systime.delay_us(1000);
    }
	
}
```

---
---
---

* 中断服务函数

为了方便移植，systick的中断服务函数没有放到irq.c中，而是直接在LQ_SYSTICK.c中
```c 
/*!
  * @brief    systime 中断服务函数
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2019/6/10 星期一
  */
void SysTick_Handler(void)
{
	/* 用户代码 */
}

```
----
----
----

### 函数指针

>>>看过例程的小伙伴可能会发现，上面介绍的API函数，例程里面都没有用到啊？使用的都是这种 systime.init();   其实只是简单的使用了函数指针，C语言基础扎实的小伙伴应该都见过这种用法。


函数指针是指向函数的指针变量，因此“函数指针”本身首先应是指针变量，只不过该指针变量指向函数。这正如用指针变量可指向整型变量、字符型、数组一样，这里是指向函数。C在编译时，每一个函数都有一个入口地址，该入口地址就是函数指针所指向的地址。有了指向函数的指针变量后，可用该指针变量调用函数，就如同用指针变量可引用其他类型变量一样，在这些概念上是大体一致的。函数指针有两个用途：  

    调用函数和做函数的参数。  

函数指针的声明方法为：

    返回值类型 ( * 指针变量名) ([形参列表]);

在LQ_SYSTICK.h中定义了一个systime_t数据类型，里面有5个函数指针，如果熟悉面向对象，会感觉这个和类的定义很像。
```c
/** 
  * @brief 一个系统时间结构体
  */
typedef struct
{	
	void (* init) (void);  
	uint32_t (* get_time_us) (void);
	uint32_t (* get_time_ms) (void);
	void (* delay_us)(uint32_t);
	void (* delay_ms)(uint32_t);
}systime_t;
```

为什么要搞这么多花里胡哨的？好处就是将同一模块的方法封装在一起，当程序规模比较大时，这种面向对象的思路就会体现出优势，这里只是提出来让大家知道有这个东西，当以后有需求的时候可以回过头来看。


在LQ_SYSTICK.c中定义了systime_t类型的变量systime 并且将5个函数指针赋值，当使用systime.get_time_us()的时候，其实就是调用systime_get_current_time_us()函数。

```c
/** 创建一个systck实例 */
systime_t  systime = 
{
	systime_init,
	systime_get_current_time_us,
	systime_get_current_time_ms,
	systime_delay_us,
	systime_delay_ms
};

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

* 初始化LED和systick 并记录开始时间

```c
    LED_Init();                          //LED初始化
    systime.init();                      //开启systick定时器
    uint32_t nowtime = 0;
    nowtime = systime.get_time_us();
```

* 在循环中，不断获取当前时间，如果当前时间与nowtime差大于1000us，则更新nowtime的值并翻转红灯。

```c
while(1)
    {
        if(systime.get_time_us() - nowtime >= 1000)
        {
            nowtime = systime.get_time_us();
            
            LED_ColorReverse(red);     //翻转红灯 用于指示程序运行
        }
    }
```

中断服务函数
```c
/*!
  * @brief    systime 中断服务函数
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2019/6/10 星期一
  */
void SysTick_Handler(void)
{
	timer.millisecond += timer.ms_per_tick;
}


```
### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个；
2) LPC54606 迷你 子母板一套；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
2) 打开配套程序，连接下载器；
3) 打开串口助手DAPLink的Com口；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
示波器观察 LED_R_PIN  P0_8  500hz


## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 

注意一般情况下为了系统实时性中断服务函数是尽可能短的，但是一些同学喜欢在中断服务函数中做处理，而systick定时器默认中断优先级是最低的，不会打断其他中断服务函数，因此如果想要在中断服务函数中使用systick，需要提高systick的中断优先级，可以自行修改。