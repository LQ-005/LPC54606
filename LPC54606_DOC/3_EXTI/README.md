[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# 外部中断例程

> 点亮小灯，是GPIO输出功能，按键则是输入。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [中断简介](###中断简介) 
    - [中断优先级分组](####中断优先级分组) 
    - [开启中断和设置优先级](####开启中断和设置优先级) 
    - [中断服务函数](####中断服务函数) 
    - [总中断开关](####总中断开关) 
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

### 中断简介

>中断是指计算机运行过程中，出现某些意外情况需主机干预时，机器能自动停止正在运行的程序并转入处理新情况的程序，处理完毕后又返回原被暂停的程序继续运行。

LPC54606一共有8个外部中断，所有管脚都可以连接到这8个外部中断上。

#### 中断优先级分组

>Reset、 NMI、 Hard Fault 优先级为负数， 高于普通中断优先级， 且优先级不可配置。

使用中断之前，首先要进行优先级分组（如果不分组，默认为组4）Cortex M内核中，每个中断都有一个8位的优先级设置寄存器，这个8位的寄存器可以分为抢占优先级和子优先级两个部分（通过设置优先级组设置）。抢占优先级和子优先级有什么用？举例说明：  

    有两个中断A、B ，A的抢占优先级比B的高，当B发生中断，正在执行B的中断服务函数时，A发生了中断，这个时候，会跳转到A的中断服务函数，A的中断服务函数执行完毕后，再接着执行B的中断服务函数，这就是中断抢占。  

	子优先级的作用是当两个中断的抢占优先级相同时，这两个中断同时发生（或者都被挂起的状态下）子优先级高的先执行，等子优先级高的执行完后再执行子优先级低的，由于抢占优先级一样，不会发生中断抢占。  

虽然这个是8位的寄存器，但是一般的单片机都不会使用全部8位，规定最少使用高3位（具体可以参考《Cortex-M3权威指南》）LPC54606、LPC55S69的优先级就只用了高3位，而K60、K66、KV58、RT1052、RT1064都是用了高4位。
```c
/* 中断优先级组 */
#define NVIC_Group0   0x07
#define NVIC_Group1   0x06
#define NVIC_Group2   0x05
#define NVIC_Group3   0x04

/* 配置优先级分组 3: 8个抢占优先级 0-7  0优先级最高 */
NVIC_SetPriorityGrouping(NVIC_Group3);
```

优先级组 | 抢占优先级 | 字优先级
--------|------------|--------
中断优先级组0| 0个抢占优先级| 8个子优先级 
中断优先级组1| 2个抢占优先级| 4个子优先级 
中断优先级组2| 4个抢占优先级| 2个子优先级 
中断优先级组3| 8个抢占优先级| 0个子优先级 

#### 开启中断和设置优先级

>一般来讲Cortex-M系列的单片机是由两个部分组成，ARM内核+芯片外设，芯片厂家拿的ARM内核授权后，再加上自己的外设（定时器之类的东西）就变成了我们看到的单片机。因此使能中断一共需要两道开关，可以大致理解为一个是芯片厂家外设对中断的使能，一个是ARM内核对中断的使能。  

调用PIN_Exti() 函数可以使能GPIO外设的中断，但是这个时候中断并没有开启，还需要使能内核部分对应的中断。  

ARM内核对中断的使能之前，最好先设置中断优先级（如果不设置，默认优先级最低），ARM Contex M内核的芯片可以通过NVIC_SetPriority()函数设置优先级，通过EnableIRQ()函数开启中断。

NVIC_SetPriority()函数有两个参数，分别是中断号和优先级。中断号是一个定义在LPC54606.h中的枚举体，可以通过选择PIN_INT1_IRQn 右键 go to definition找到该枚举体，如果使用其他中断，都可以在枚举体中找对应中断号。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBf0b698d2e831b0c3bd1d68e39b7efed1?method=download&shareKey=1ab5b5907c2b57f2bf1a2263229dc6d3">
</div>

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB031d27157f91192f1d65fa15fe6bc26e?method=download&shareKey=1cd61db81a494391934386c603d633a6">
</div>

第二个参数优先级，由于优先级与优先级分组有关，所以使用NVIC_EncodePriority()函数对优先级进行编码转化为寄存器对应的位，NVIC_EncodePriority()函数有三个参数，分别是优先级组、抢占优先级、子优先级。优先级组可以通过NVIC_GetPriorityGrouping()函数获取，抢占优先级和子优先级需要我们自行设置。

```c
//优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级可打断别的中断
NVIC_SetPriority(PIN_INT1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
```

EnableIRQ()函数开启中断,

```c
EnableIRQ(PIN_INT0_IRQn);
```

#### 中断服务函数

中断服务函数声明一般在启动文件中，打开对应启动文件（后缀为.s的汇编文件），找到对应的函数名并将其复制到irq.c文件中。中断服务函数可以放在工程任意.c文件中，这里为了方便管理自己的中断服务函数特意将其放在irq.c文件中。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBf1118f0990b69fe3e68766c2aa85754e?method=download&shareKey=7664ded833f65bb5ffcfdd02113e9c7a">
</div>

#### 总中断开关
在程序开始阶段，可以关闭全局中断 DisableInterrupts  当所有外设都初始化完成后再开启全局中断 EnableInterrupts

避免出现中断服务函数中操作未初始化外设.

例如初始化了MRT定时器 在MRT中断中使用串口1打印，但是MRT先初始化，进入中断时串口1还未初始化，就导致了操作未初始化的串口1，就会进入这里

Cortex-M内核的单片机可以通过修改内核寄存器完成开关所有中断操作。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB3e99b05d399e1e5d37b4f3985be4fa88?method=download&shareKey=e341ff4b89e4203da67db4d4c554a2c4">
</div>


### 常用API

>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>

* 外部中断初始化

GPIO外部中断初始化函数主要的参数是mode，可以选择合适的上下拉电阻。比如按键电路，母板上可以去掉外部的4.7K上拉电阻，使用内部上拉的方式。在LQ_GPIO.h中，已经将GPIO常用的模式通过宏定义方式写出，使用PIN_Exti()函数时，mode参数直接从宏定义选择即可，也可以自行配置。LPC54606一共有8个外部中断，所有管脚都可以连接到这8个外部中断上，参数Channel确定管脚具体连接到哪一个外部中断。
```c

/* GPIO 中断触发模式 */
#define PIN_IRQ_MODE_RISING             0x01 /*!< 上升沿（下拉）触发中断 */  
#define PIN_IRQ_MODE_FALLING            0x02 /*!< 下降沿（上拉）触发中断 */
#define PIN_IRQ_MODE_RISING_FALLING     0x03 /*!< 双边沿（开漏）触发中断 */
#define PIN_IRQ_MODE_LOW_LEVEL          0x04 /*!< 低电平（上拉）触发中断 */
#define PIN_IRQ_MODE_HIGH_LEVEL         0x05 /*!< 高电平（下拉）触发中断 */


/*!
  * @brief    GPIO外部中断初始化
  *
  * @param    pin      ： 管脚标号 LQ_GPIO_Cfg.h中定义
  * @param    mode     ： GPIO 中断模式   LQ_GPIO.h中的宏定义中选择
  * @param    Channel  ： 使用的外部中断通道 0-7
  *
  * @return   无
  *
  * @note     LPC54606一共有8个外部中断，所有管脚都可以连接到这8个外部中断上 
  *
  * @see      PIN_Exti(P0_0, 0, PIN_IRQ_MODE_FALLING);//配置P0_0 下降沿触发中断
  * @see      优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级可打断别的中断
  * @see      NVIC_SetPriority(PIN_INT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
  * @see      EnableIRQ(PIN_INT0_IRQn);			          //使能外部中断通道 0   
  *
  * @date     2019/8/19 星期一
  */
void PIN_Exti(GPIO_Name_t pin, uint8_t Channel, uint8_t mode)
```

* 中断服务函数

根据PIN_Exti()时的Channel参数，确定对应的中断服务函数
Channel值 | 中断服务函数
----------|------------
0|PIN_INT0_IRQHandler
1|PIN_INT1_IRQHandler
2|PIN_INT2_IRQHandler
3|PIN_INT3_IRQHandler
4|PIN_INT4_IRQHandler
5|PIN_INT5_IRQHandler
6|PIN_INT6_IRQHandler
7|PIN_INT7_IRQHandler

```c
/**
  * @brief    PIN_INT1_IRQHandler中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     测试外部中断例程使用该函数
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void PIN_INT1_IRQHandler(void)
{ 
    /* 用户代码 */

    PINT_PinInterruptClrStatus(PINT, kPINT_PinInt1);      //清除中断标志位
}

```

* 关闭中断

这个函数时corte-m内核文件自带的，corte-m内核的单片机都可以使用这个函数，函数参数为中断号。

```c
DisableIRQ(PIN_INT0_IRQn);
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

* 初始化按键相连的IO为外部中断模式，并且初始化LED灯

```c
    LED_Init();
    
    /* 配置下降沿触发中断 */
    PIN_Exti(KEY0_PIN, 0, PIN_IRQ_MODE_FALLING);
    PIN_Exti(KEY1_PIN, 1, PIN_IRQ_MODE_FALLING);
    PIN_Exti(KEY2_PIN, 2, PIN_IRQ_MODE_FALLING);
  
    //优先级配置 抢占优先级4  子优先级0   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(PIN_INT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4,0));
    
	EnableIRQ(PIN_INT0_IRQn);			          //使能PIN_INT0_IRQn的中断  
    
    //优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(PIN_INT1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
    
	EnableIRQ(PIN_INT1_IRQn);			          //使能PIN_INT1_IRQn的中断 

    //优先级配置 抢占优先级6  子优先级0   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(PIN_INT2_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),6,0));
    
	EnableIRQ(PIN_INT2_IRQn);			          //使能PIN_INT2_IRQn的中断
```

* while(1)等待，当按键按下，进入中断服务函数，在中断服务函数中翻转LED

```c
/**
  * @brief    PIN_INT0_IRQHandler中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     测试外部中断例程使用该函数
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void PIN_INT0_IRQHandler(void)
{ 
    /* 用户代码 */
    LED_ColorReverse(red);

    PINT_PinInterruptClrStatus(PINT, kPINT_PinInt0);      //清除中断标志位

    __DSB();				                              //数据同步屏蔽指令 
}


/**
  * @brief    PIN_INT1_IRQHandler中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     测试外部中断例程使用该函数
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void PIN_INT1_IRQHandler(void)
{ 
    /* 用户代码 */
    LED_ColorReverse(green);

    PINT_PinInterruptClrStatus(PINT, kPINT_PinInt1);      //清除中断标志位

    __DSB();				                              //数据同步屏蔽指令 
}


/**
  * @brief    PIN_INT2_IRQHandler中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     测试外部中断例程使用该函数
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void PIN_INT2_IRQHandler(void)
{ 

    /* 用户代码 */
    LED_ColorReverse(blue);

    PINT_PinInterruptClrStatus(PINT, kPINT_PinInt2);      //清除中断标志位

    __DSB();				                              //数据同步屏蔽指令 
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