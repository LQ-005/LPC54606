[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# UART例程

> UART是一种常用的通信方式，做嵌入式必须要有一定的了解。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [UART简介](###UART简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)


## 硬件电路

>>UART是单片机常用的通信口，需要注意通信两端的通信格式需要保持一致（波特率、数据位、停止位、校验）。

我们常用的UART需要三根接线，TX、RX和GND。

TX | P0_30| 发送
--|--|--
RX | P0_29 | 接收

两个串口相连时需要注意，一个串口的发送需要接另一个串口的接收，也就是常说的TX接RX。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB22835fb94990fd12ff0bb68ef705984a?method=download&shareKey=4244729ab7903ddf4324740f5b689205">
</div>

## 软件实现

>>这是一个简易的教程，只是为了让大家快速入门，熟悉相应的API，调用对应API实现自己的功能即可。如果不是做嵌入式底层的，没有太大必要去深究寄存器，毕竟现在嵌入式的趋势是将底层和上层分开，减少用户的开发周期。

### UART简介

LPC54606 有9个FlexCOM口，这九个FlexCOM口都可以变身成为串口，因此串口的中断函数其实是FlexCOM的中断函数。

### 常用API

>>>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>


* Console UART初始化
>>>官方SDK中的Console调试控制台在调试时非常有用，因此在新建工程时将Console 也移植到工程中了。

在时钟初始化完后，都会进行Console初始化，这里控制台默认使用的是串口0
```c
/* 初始化串口0 可以使用 PRINTF函数 */
    BOARD_InitDebugConsole();
```

Console的默认参数可以修改board.h中的宏定义进行修改。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBf8945e842639f664cbdbb85bc1f28a47?method=download&shareKey=3e47cd58e63445692ffe9463dd4c68dc">
</div>

Console默认管脚  可以在LQ_UART.h中通过宏定义修改。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBea6cfe0004eeccc4152aac8527095574?method=download&shareKey=1cdd5fae12326d8797c6cb29f2c15b34">
</div>

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

* 循环接收字符并打印出去

```c
    uint8_t ch;
    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
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
打印 北京龙邱智能科技. 并且打印接收到的字符串

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB1ff0a82b6497b4bda5416a9bf04376ed?method=download&shareKey=f4f57023893ad20b095bd5a5bb5391dd">
</div>

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 