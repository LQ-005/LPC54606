[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# 超声波例程

>超声波是一种根据声速测量距离的模块，超声波模块会发射超声波，当超声波遇到障碍物后会反射回来，只要获取超声波从发射到接收到反射之间的时间，就可以通过声速计算出距离障碍物的距离，因为声速有限，所以超声波测距频率不应过高。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [超声波简介](###超声波简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)

## 硬件电路

HCSR04超声波模块，只需要任意两个IO即可。例程中使用如下接线。

     Vcc          ---  3.3V/5V
     GND          ---  GND
     HCSR04_TRIG  ---  P0_7
     HCSR04_ECHO  ---  P1_13

## 软件实现

### 超声波简介

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB83f874399ed8ecbc1b7b0f5a3286bdbb?method=download&shareKey=8fb7a098e90cacc6bcfbf1ce9d1d6c5f">
</div>

默认情况下，TRIG和ECHO都为低电平，给TRIG管脚添加一个8-15us的高电平信号，超声波模块就会发送超声波，同时拉高ECHO管脚，当接受到反射回来的超声波信号后，ECHO管脚拉低。记录ECHO高电平时间则可以根据声速算出距离:  

     距离 = （超声波传输时间/2）*340m/s

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB7a6c6b029cca6658c2abe10711baf05d?method=download&shareKey=207c2fc4c18f3f567795b0efb1cb3902">
</div>

### 常用API

>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>

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

* 根据超声波的原理，只要记录ECHO高电平时间即可算出距离信息。因此可以开启与ECHO管脚相连的GPIO双边沿中断，当ECHO上升沿时记录当前时间为起始时间，当ECHO下降沿时，使用当前时间，从而计算出ECHO高电平时间。中断服务函数在irq.c中。

>OLED和TFT18都可以使用，具体使用那个屏显示，请打开config.h，选择对应的显示屏。

```c
 /**  hc_starttime 记录超声波测距ECHO 高电平开始时间  hc_time 记录ECHO高电平持续时间 */
volatile uint32_t hc_starttime; 
volatile uint16_t hc_time;

systime.init();                      //开启systick定时器
	LED_Init();
	PRINTF("\r\nLQ HC-SR04 Test \n");
	
#ifdef OLED
	LCD_Init();
    LCD_CLS();
	LCD_P8x16Str(5,0,"LQ HC-SR04 Test");
#else
	TFTSPI_Init(1);                //TFT1.8初始化  0：横屏显示  1：竖屏显示  
    TFTSPI_CLS(u16BLUE);           //清屏
	TFTSPI_P8X16Str(0,0,"LQ HC-SR04 Test",u16RED,u16BLUE);
#endif
	char txt[16];
	uint32_t now_time = systime.get_time_us();
	static uint8_t count = 0;
	float dis = 0;
    
    
	/* 初始化超声波的IO */
	PIN_InitConfig(HCSR04_TRIG, PIN_MODE_OUTPUT, 0);
    PIN_Exti(HCSR04_ECHO, 4, PIN_IRQ_MODE_RISING_FALLING);
    
    //优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(PIN_INT4_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
    
	EnableIRQ(PIN_INT4_IRQn);			 //使能中断  
    /*GPIO 外部中断配置结束*/  
```

* 在循环中，不断发送超声波，并根据中断中记录的时间显示距离信息，注意测距频率最好不最大20Hz

```c
	while(1)
	{       
		if(systime.get_time_us() - now_time >= 50000)
		{
			now_time = systime.get_time_us();
            
			/* HC_TRIG 保持8-15us的高电平 */
			PIN_Write(HCSR04_TRIG, 1);
			systime.delay_us(15);
			PIN_Write(HCSR04_TRIG, 0);
			
		
			/* 计算距离 hc_time/2 * 340m/s */
			dis = (float)(hc_time * 0.017);          //计算距离dis
			
			if(count++ % 5 == 0)
			{
				sprintf(txt, "Dis:%5.2fcm ", dis);
				PRINTF(txt);
                PRINTF("\n");
				LED_ColorReverse(green);
#ifdef OLED
				LCD_P8x16Str(0,3,txt);
#else
				TFTSPI_P8X16Str(2,3,txt,u16RED,u16BLUE);
#endif
			}
		}
	}
```
* 中断服务函数

```c
/**
  * @brief    PIN_INT4_IRQHandler中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     超声波测距
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void PIN_INT4_IRQHandler(void)
{ 
    /* 用户代码 */
    if(PIN_Read(HCSR04_ECHO))
    {
        hc_starttime = systime.get_time_us();
    }
    else
    {
        hc_time = systime.get_time_us() - hc_starttime;
    }

    PINT_PinInterruptClrStatus(PINT, kPINT_PinInt4);      //清除中断标志位

    __DSB();				                              //数据同步屏蔽指令 
}
```
---
---
---

### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个；
2) LPC54606 迷你 子母板一套+TFT18/OLED一块+HCSR04一个；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
2) 打开配套程序，连接下载器；
3) 插上TFT18/OLED，接好超声波；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
屏幕上会显示距离信息。

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 
