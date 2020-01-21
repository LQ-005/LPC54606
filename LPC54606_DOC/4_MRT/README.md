[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# MRT定时器例程

> 大部分任务都是周期性执行的，比如LED周期性闪烁，PID周期控制等，定时器精度高，无需消耗CPU等优点成为划分周期的依据。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [MRT简介](###MRT简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)


## 硬件电路

>定时器封住在单片机内部，是单片机的重要资源。

## 软件实现

>这是一个简易的教程，只是为了让大家快速入门，熟悉相应的API，调用对应API实现自己的功能即可。如果不是做嵌入式底层的，没有太大必要去深究寄存器，毕竟现在嵌入式的趋势是将底层和上层分开，减少用户的开发周期。

### MRT简介

MRT是一个4通道24位的定时器，使用系统时钟源（默认180MHz）。

### 常用API

>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>


* MRT初始化
>定时器精度高，无需CPU参与，当主频180MHz时，MRT每隔 1/180M秒 跳动一次（向上计数就是加一，向下计数就是减一），定时器的跳动周期是固定的，因此可以进行计时和延时功能。

MRT有4个可编程周期触发中断，因此一般情况下将MRT当作周期中断触发器使用，当周期中断触发器使用时，只需要对MRT进行初始化，确定中断周期，然后打开中断就可以周期性进入中断服务函数。关于中断优先级配置和使能，可以参考外部中断章节。
```c
/**
  * @brief    MRT定时中断初始化
  *
  * @param    channel ： MRT通道 kMRT_Channel_0 - kMRT_Channel_3
  * @param    us      ： 定时中断时间
  *
  * @return   无
  *
  * @note     MRT最大 0XFFFFFF  最大计时90ms   
  *
  * @see      MRT_InitConfig(kMRT_Channel_0, 5000); //5ms触发一次MRT中断
  * @see      优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级高的可打断抢占优先级低的中断
  * @see      NVIC_SetPriority(MRT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
  * @see      EnableIRQ(MRT0_IRQn);  //使能中断
  *
  * @date     2019/5/6 星期一
  */
void MRT_InitConfig(mrt_chnl_t channel, uint32_t us)
```
---
---
---

* 中断服务函数

MRT有4个可编程周期触发中断，公用一个中断服务函数。中断服务函数可以放在工程任意.c文件中，这里为了方便管理自己的中断服务函数特意将其放在irq.c文件中。

```c
/*!
  * @brief    MRT中断服务函数
  *
  * @param    无
  *
  * @return   
  *
  * @note     Test_MRT()函数使用
  *
  * @see      
  *
  * @date     2019/10/31 星期四
  */
void MRT0_IRQHandler(void)
{
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_0) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_0, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */

    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_1) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_1, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */

    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_2) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_2, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */

    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_3) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_3, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */
    }
}

```

* 关闭MRT其中一个通道
>龙邱的LPC54606是基于官方fsl库的，fsl库已经将底层封装好了，只是因为功能太多，太过灵活，对于新手来说不好上手。我们将比较常用的功能封装起来，保留常用的参数，形参龙邱的上层库.

如果开启了多个MRT中断，想要关掉其中一个，可以通过屏蔽对应的中断，或者直接停止对应通道计数的方法关闭中断。有些同学可能会疑问，在龙邱的库中没找到对应的关闭函数呀？ 这些函数直接调用fsl库函数即可，龙邱并没有对这些可以直接使用的函数进行封装。

fsl库函数中 Enable和Disable基本都是成对出现的，使用gotodef功能跳转到对应代码段附近查找Disable函数。函数的参数可以参考Enable部分。 
<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB95d1395a3e1d14a518305efdaf4f0445?method=download&shareKey=c2985fea8ece6cf606254c716ba64b7a">
</div>

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB417c47fdc52d1c7eddd3c5c11a51d7b9?method=download&shareKey=0945a5da31c794d1a9c5b472092ee077">
</div>

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB96f6b5b10b1a1ea7f53488c8e3d524af?method=download&shareKey=71993277f223dacf71b21a67d8acf8a1">
</div>

这个函数时corte-m内核文件自带的，corte-m内核的单片机都可以使用这个函数，函数参数为中断号。

```c
DisableIRQ(MRT0_IRQn);
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

* 初始化IO为输出模式，初始化LED灯并且初始化MRT通道中断周期

```c
/* 初始化 LED */
    LED_Init();
    
    /* 初始化 IO 在中断服务函数中反转 方便观察是否进入中断 */
    PIN_InitConfig(P0_26, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(P1_17, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(P0_28, PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(P0_27, PIN_MODE_OUTPUT, 0);
    
    /* 设置MRT定时器通道0 1000us触发一次中断 */
    MRT_InitConfig(kMRT_Channel_0, 1000);
    
    /* 设置MRT定时器通道0 5000us触发一次中断 */
    MRT_InitConfig(kMRT_Channel_1, 5000);
    
    /* 设置MRT定时器通道0 10000us触发一次中断 */
    MRT_InitConfig(kMRT_Channel_2, 10000);
    
    /* 设置MRT定时器通道0 50000us触发一次中断 */
    MRT_InitConfig(kMRT_Channel_3, 50000);
    
    //优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级高的可打断抢占优先级低的中断
    NVIC_SetPriority(MRT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
    EnableIRQ(MRT0_IRQn);  //使能中断
    
    /* 在irq.c中 编写中断服务函数*/
    while(1);
```
---
---
---
* while(1)等待，当定时周期到来，进入中断服务函数，在中断服务函数中翻转对应IO

```c
/*!
  * @brief    MRT中断服务函数
  *
  * @param    无
  *
  * @return   
  *
  * @note     Test_MRT()函数使用
  *
  * @see      
  *
  * @date     2019/10/31 星期四
  */
void MRT0_IRQHandler(void)
{
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_0) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_0, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */
        PT0_26 = !PT0_26;               //取反
    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_1) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_1, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */
        PT1_17 = !PT1_17;               //取反
    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_2) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_2, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */
        PT0_28 = !PT0_28;
    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_3) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_3, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */
        PT0_27 = !PT0_27;
        LED_ColorReverse(blue);    //蓝灯
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
示波器测试 P0_26 P1_17 P0_28 P0_27 频率分别500Hz 100Hz 50Hz 10Hz  

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 
