[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# printf例程

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


* UART初始化
>>>串口的参数比较多，这里的串口初始化只保留了波特率参数，其他的在串口初始化内部使用默认值，可以自行修改。

```c
/*!
  * @brief    串口模块初始化
  *
  * @param    uratn ： USART0 - USART8
  * @param    bdr   ： 波特率
  *
  * @return   无
  *
  * @note     这里实际是将flexcom口用作串口，如果使用了flexcom0当作串口0，那么flexcom0 就不可用作IIC0，SPI0了
  * 
  * @see      UART_InitConfig(USART0, 115200);   //初始化串口0 波特率 115200 无奇偶校验 1停止位 使用管脚LQ_UART.h中通过宏定义选取
  *
  * @date     2019/10/19 星期六
  */
void UART_InitConfig(USART_Type * uratn,uint32_t bdr)
```

UART管脚  可以在LQ_UART.h中通过宏定义修改。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBea6cfe0004eeccc4152aac8527095574?method=download&shareKey=1cdd5fae12326d8797c6cb29f2c15b34">
</div>

---
---
---

* 输出一个字符

```c
/*!
  * @brief    串口打印一个字符
  *
  * @param    uratn ： USART0 - USART8 
  * @param    ch    ： 要打印的字符
  *
  * @return   无
  *
  * @note     使用前请先初始化对应串口
  *
  * @see      UART_PutChar(USART0, 'a');  //打印字符a
  *
  * @date     2019/10/19 星期六
  */
void UART_PutChar(USART_Type * uratn, char ch)
```

---
---
---

* 输出字符串

```c
/*!
  * @brief    串口打印字符串
  *
  * @param    uratn ： USART0 - USART8 
  * @param    str   ： 要打印的字符串地址 
  *
  * @return   无
  *
  * @note     使用前请先初始化对应串口
  *
  * @see      UART_PutStr(USART4, "123456789"); //发送9个字节      
  *
  * @date     2019/10/21 星期一
  */
void UART_PutStr(USART_Type * uratn, char *str)

/*!
  * @brief    打印已知长度的数组
  *
  * @param    uratn ： USART0 - USART8 
  * @param    buff  ： 要打印的字符串地址    
  * @param    len   ： 要打印的长度
  *
  * @return   无
  *
  * @note     使用前请先初始化对应串口
  *
  * @see      UART_PutBuff(USART4, "123456789",5);//实际发送5个字节‘1’‘2’‘3’‘4’‘5’    
  *
  * @date     2019/10/21 星期一
  */
void UART_PutBuff(USART_Type *base, uint8_t *buff, uint32_t len)
```

---
---
---

* printf

>>>大多数同学比较习惯使用printf，需要对printf进行重映射。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBcb73f3e99375271b72f1899df312eaae?method=download&shareKey=a9f2eb9f0319362d4bcfd72d41ecdec4">
</div>

---
---
---

* 获取一个字符
>>>一般用作串口接收中断中,注意只有接收到字符才会退出，否则会一直等待接收
```c
/*!
  * @brief    接收一个字符
  *
  * @param    uratn ： USART0 - USART8   
  *
  * @return   接收到的字符
  *
  * @note     使用前请先初始化对应串口
  *
  * @see      UART_GetChar(USART4); //接收一个字符
  *
  * @date     2019/10/21 星期一
  */
char UART_GetChar(USART_Type * uratn)

```

---
---
---

* 中断服务函数

>>>上面的API都是输出的，如果想要接收字符，最常用的是使用接收中断，在接收中断中接收字符，将字符缓存起来在进行解析，可以参考匿名上位机的协议。

串口初始化函数会默认打开外设部分的串口接收中断（可以参考外部中断章节），这里我们只要设置中断优先级，打开内核对应的中断即可开启函数中断。
```c 
/* 使能串口0接收中断 */
//优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级高的可打断抢占优先级低的中断
NVIC_SetPriority(FLEXCOMM0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
EnableIRQ(FLEXCOMM0_IRQn);  //使能中断  FLEXCOMM0用作USART0了，所以使能中断使能的是FLEXCOMM0

/*!
  * @brief    flexcom0中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     测试串口使用
  *
  * @see      
  *
  * @date     2019/10/31 星期四
  */
void FLEXCOMM0_IRQHandler(void)
{
    /* 接收中断. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(USART0))
    {
        /* 用户代码 */
    } 
}

```
----
----
----

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

* 初始化串口使能中断

```c
/* 初始化串口0 */
    UART_InitConfig(USART0, 115200);
    
    /* 初始化串口3 */
    UART_InitConfig(USART3, 115200);
    
    /* 使能串口0接收中断 */
    //优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级高的可打断抢占优先级低的中断
    NVIC_SetPriority(FLEXCOMM0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
    EnableIRQ(FLEXCOMM0_IRQn);  //使能中断  FLEXCOMM0用作USART0了，所以使能中断使能的是FLEXCOMM0

    /* 使能串口3接收中断 */
    //优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级高的可打断抢占优先级低的中断
    NVIC_SetPriority(FLEXCOMM3_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
    EnableIRQ(FLEXCOMM3_IRQn);  //使能中断  FLEXCOMM3用作USART3了，所以使能中断使能的是FLEXCOMM3
    
    /* printf 使用串口0 可以在LQ_UART.c中修改 fputc 函数修改printf的串口 */
    printf("\n\r串口打印测试\n\r");
```

* 循环打印字符串，如果接收到字符会进入中断服务函数并将接收到的字符打印出来

```c
while(1)
    {
        UART_PutStr(USART0, "123456789\r\n");
        UART_PutStr(USART3, "123456789\r\n");
        
        UART_PutBuff(USART0, "123456789",5);
        UART_PutBuff(USART3, "123456789",5);
        
        /* 发送换行 */
        UART_PutChar(USART3, 0x0d);
        
        printf("\n\r串口打印测试\n\r");
        delayms(2000);
    }
```

中断服务函数
```c
/*!
  * @brief    flexcom0中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     测试串口使用
  *
  * @see      
  *
  * @date     2019/10/31 星期四
  */
void FLEXCOMM0_IRQHandler(void)
{
    uint8_t data;
    /* 接收中断. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(USART0))
    {
        data = UART_GetChar(USART0);
        printf("接收到字符： %c \n", data);
    } 
}


/*!
  * @brief    flexcom3中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     测试串口使用
  *
  * @see      
  *
  * @date     2019/10/31 星期四
  */
void FLEXCOMM3_IRQHandler(void)
{
    char txt[16];
    uint8_t data;
    /* 接收中断. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(USART3))
    {
        data = UART_GetChar(USART3);
        sprintf(txt, "接收到字符： %c \n", data);
        UART_PutStr(USART3, txt);
    } 
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
打印 北京龙邱智能科技. 并且打印接收到的字符

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB2cf757cbbb3061d9dc24502370b07faa?method=download&shareKey=f32a186246570813664fe56adc8c3329">
</div>

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 