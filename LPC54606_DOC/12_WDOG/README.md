[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# WDOG例程

>俗称看门狗，设置后需要定时喂狗，如果规定时间内不喂狗，单片机会复位，该功能可以防止程序卡死跑飞。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [WDOG简介](###WDOG简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)


## 硬件电路

>>WDOG是单片机的内部资源。

## 软件实现

### WDOG简介

> LPC54606的WWDT是一个24位的定时器，默认时钟源500KHz，但是WWDT有一个4分频的分频器，实际使用的时钟是125KHz。WWDT是一个窗口看门狗，有窗口上限和下限，必须在窗口下限和上限之间喂狗，否则会导致单片机复位。

### 常用API

>>>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>

* 看门狗初始化

看门狗初始化就是确定喂狗窗口时间的上限和下限，注意初始化后需要自行配置看门狗中断优先级，打开看门狗中断。

```c

/*!
  * @brief    看门狗初始化
  *
  * @param    min_ms    ：窗口下限时间 单位 ms
  * @param    max_ms    ：窗口上限时间 单位 ms
  *
  * @return   无
  *
  * @note     wwdt是一个24位的定时器 默认时钟125KHz  max_ms最大 134217
  *
  * @see      WDOG_InitConfig(2000, 5000); //设置看门狗 在2-5S内喂狗
  * @see      优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级高的可打断抢占优先级低的中断
  * @see      NVIC_SetPriority(WDT_BOD_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
  * @see      EnableIRQ(WDT_BOD_IRQn);  //使能中断
  *
  * @date     2019/10/19 星期六
  */
void WDOG_InitConfig(uint32_t min_ms, uint32_t max_ms)
```

---
---
---

* 喂狗

看门狗需要在规定时间内喂狗。

```c
WWDT_Refresh(WWDT);
```

* 中断服务函数

看门狗没有在规定时间内喂狗，则会进入看门狗中断，在中断里可以进行一些紧急处理，注意处理时间不要过长，然后单片机会进行复位。在看门狗初始化函数中，不经设置了窗口上限时间timeoutValue，还设置了警告时间warningValue，警告时间意思是在timeoutValue-warningValue时刻，没有检测到喂狗，也会进入看门狗中断，只不过看门狗中断的标志位不同，也可以进行一些处理。

```c

/**
  * @brief    看门狗中断
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @example  
  *
  * @date     2019/5/7 星期二
  */
void WDT_BOD_IRQHandler(void)
{
    uint32_t wdtStatus = WWDT_GetStatusFlags(WWDT);

    //单片机之前复位
    if (wdtStatus & kWWDT_TimeoutFlag)
    {        
        WWDT_Disable(WWDT);//停止看门狗

        /* 复位前的一些紧急处理 */

        WWDT_Enable(WWDT);//启动看门狗
    }

    //警告处理
    if (wdtStatus & kWWDT_WarningFlag)
    {
        //用户代码
        
    }
}
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

* 初始化LED（初始化后会亮白灯），初始化看门狗，使能看门狗中断，等待喂狗。

>>>OLED和TFT18都可以使用，具体使用那个屏显示，请打开config.h，选择对应的显示屏。

```c
     LED_Init();
     WDOG_InitConfig(1000, 2000);
     
     // 优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级高的可打断抢占优先级低的中断
     NVIC_SetPriority(WDT_BOD_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
     EnableIRQ(WDT_BOD_IRQn);  //使能中断
      
     /* */
     while(1);
```

* 由于主程序没有喂狗，所以到达警告时间时会进入irq.c的看门狗中断服务函数，中断服务函数中翻转红灯，并进行警告处理，这里会进行5次喂狗操作，所以可以看到红灯会翻转5次。五次警告处理喂狗过后，再次进入警告处理，但是并没有喂狗，所以超出喂狗时间后会再次进入中断服务函数，进行单片机复位之前的处理，处理完后单片机复位。

```c
/* */
static volatile uint8_t wdt_update_count = 0;


/**
  * @brief    看门狗中断
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @example  
  *
  * @date     2019/5/7 星期二
  */
void WDT_BOD_IRQHandler(void)
{
    uint32_t wdtStatus = WWDT_GetStatusFlags(WWDT);

    LED_ColorReverse(red);     //红灯闪烁

    //单片机之前复位
    if (wdtStatus & kWWDT_TimeoutFlag)
    {        
        WWDT_Disable(WWDT);//停止看门狗
        WWDT_ClearStatusFlags(WWDT, kWWDT_TimeoutFlag);//状态清零
        /* 复位前的一些紧急处理 */

        WWDT_Enable(WWDT);//启动看门狗
    }

    //警告处理
    if (wdtStatus & kWWDT_WarningFlag)
    {
        //警告之前没有喂狗
        WWDT_ClearStatusFlags(WWDT, kWWDT_WarningFlag);
        if (wdt_update_count < 5)
        {
            //5次警告后将启动看门狗复位动作
            WWDT_Refresh(WWDT);
            wdt_update_count++;
        }
    }
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
核心板上的LED灯闪烁 复位后白灯亮2s  然后红灯翻转5次，单片机复位。

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 