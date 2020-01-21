[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# SERVO例程

>舵机驱动例程，舵机驱动简单来讲就是控制PWM占空比，从而控制舵机旋转角度。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [PWM简介](###PWM简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)

## 硬件电路

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBfe25493d79640dad333216e7824acd97?method=download&shareKey=359ed3f8ffafb2dae6529027d79654b7">
</div>

S3010和SD5舵机都可以通过50Hz的PWM信号驱动，和直流电机通过PWM调节电压控制转速的原理不同，舵机一般使用5-6V的恒压供电，舵机内部可以获取PWM信号的高电平时间，根据高电平时间旋转到对应的对应角度。一般取1.5ms的高电平时间为舵机中值，调完中值之后，再将舵盘装上。（注意龙邱母板上的舵机接口电压可调，先调节滑动变阻器，将舵机电压调节到5-6V，在接舵机）

白色（黄色）|  ----- |  PWM信号线   P1_10  
--|--|--  
红色        | ----- |  5V - 6V  
黑色（棕色） | -----  | GND

## 软件实现

### PWM简介

> PWM是单片机内部定时器产生的，龙邱LPC54606VD母板上使用的舵机PWM是由CTIME1产生的，LPC54606内部有5个32位的CTIME定时器，每个CTIME可以产生3路频率相同的PWM。

### 常用API

>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>

* PWM初始化

CTIME输出PWM的初始化函数需要三个参数，第一个是输出通道（每个通道只能使用其中一个管脚）；第二个数占空比参数，占空比一般情况下是0-100%，但是这样粒度太大了，不方便控制，因此使用宏定义CMTER_PWM_MAX，定义占空比最大值，默认是10000,当参数duty是1000时，实际占空比是1000/CMTER_PWM_MAX * 100；第三个参数时输出PWM频率，电机一般使用10-20Khz，舵机一般使用50Hz（频率高了可以加快舵机反应速度，但是舵机可能会发出嗡嗡声）；

```c

/*!  CTIMER 最大占空比 可自行修改 */
#define  CMTER_PWM_MAX    10000

/** 
  * @brief CTIMER模块 PWM输出通道 一个CT最多输出3路周期相同的PWM，四个通道中通道3用作周期通道
  * @note  CTIMER 用做PWM输出通道 一个CTIMER模块只能用作脉冲计数和PWM输出中的一种
  */ 
typedef enum  
{
    //CTIMER PWM通道 ，一行最多选一个（一个CT最多输出3路周期相同的PWM，四个通道中通道3用作周期通道）     
    CT0Output0_P0_0 = 1 + 0x0000, CT0Output0_P0_30 = 2 + 0x0000,  /*!< CTIME0 PWM输出通道0管脚  */    
    CT0Output1_P0_3 = 1 + 0x0100, CT0Output1_P0_31 = 2 + 0x0100,  /*!< CTIME0 PWM输出通道1管脚  */       
    CT0Output2_P0_19= 1 + 0x0200, CT0Output2_P1_31 = 2 + 0x0200,  /*!< CTIME0 PWM输出通道2管脚  */       
 
    CT1Output0_P0_18= 1 + 0x1000, CT1Output0_P1_10 = 2 + 0x1000,  /*!< CTIME1 PWM输出通道0管脚  */   
    CT1Output1_P0_20= 1 + 0x1100, CT1Output1_P1_12 = 2 + 0x1100,  /*!< CTIME1 PWM输出通道1管脚  */   
    CT1Output2_P0_23= 1 + 0x1200, CT1Output2_P1_14 = 2 + 0x1200,  /*!< CTIME1 PWM输出通道2管脚  */   

    CT2Output0_P0_10= 1 + 0x2000, CT2Output0_P1_5  = 2 + 0x2000,  /*!< CTIME2 PWM输出通道0管脚  */  
    CT2Output1_P1_6 = 1 + 0x2100, CT2Output1_P1_4  = 2 + 0x2100,  /*!< CTIME2 PWM输出通道1管脚  */  
    CT2Output2_P1_7 = 1 + 0x2200, CT2Output2_P0_11 = 2 + 0x2200,  /*!< CTIME2 PWM输出通道2管脚 P0_11（下载口）   */
    
    CT3Output0_P0_5 = 1 + 0x3000,                                 /*!< CTIME3 PWM输出通道0管脚  */
    CT3Output1_P1_19= 1 + 0x3100,                                 /*!< CTIME3 PWM输出通道1管脚  */
    CT3Output2_P0_27= 1 + 0x3200, CT3Output2_P1_21 = 2 + 0x3200,  /*!< CTIME3 PWM输出通道2管脚  */
    
    CT4Output0_P0_6= 1 + 0x4000,                                  /*!< CTIME4 PWM输出通道0管脚  */  
} CTIMER_OutputChannel_t;

/*!
  * @brief    初始化CTIMER为PWM输出功能
  *
  * @param    matchChannel   ：    CTIMER通道  LQ_CTIMER.h中枚举体
  * @param    duty           ：    占空比 * CMTER_PWM_MAX
  * @param    pwmFreq_Hz     ：    频率
  *
  * @return   无
  *
  * @note     一个CTIMER模块只能用作脉冲计数和PWM输出的一种
  * @note     一个CTIMER模块最多只能输出3路PWM频率相同的PWM波
  *
  * @see      CTIMER_InitPwmConfig(CT0Output0_P0_0, 5000, 100);//初始化P0_0 作为CTIMER0的通道0 的PWM输出口 频率100Hz 占空比 百分之(5000/CMTER_PWM_MAX)*100
  *
  * @date     2019/5/7 星期二
  */
void CTIMER_InitPwmConfig(CTIMER_OutputChannel_t matchChannel, uint32_t duty, uint32_t pwmFreq_Hz);

```

---
---
---

* 设置占空比

```c
/*!
  * @brief    CTIMER 更新PWM占空比
  *
  * @param    matchChannel   ：    CTIMER通道
  * @param    duty           ：    占空比 * CMTER_PWM_MAX
  *
  * @return   无
  *
  * @note     使用前请先初始化对应通道
  *
  * @see      CTIMER_SetDuty(CT0Output0_P0_0, 4000); //设置P0_0 通道 PWM占空比 百分之 百分之(4000/CMTER_PWM_MAX)*100 
  *
  * @date     2019/5/7 星期二
  */
void CTIMER_SetDuty(CTIMER_OutputChannel_t matchChannel, uint32_t duty);

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

* 初始化PWM，初始化LED（初始化后会亮白灯），初始化按键，初始化屏幕。

>OLED和TFT18都可以使用，具体使用那个屏显示，请打开config.h，选择对应的显示屏。

```c
    /* 舵机PWM 初始化 */
    CTIMER_InitPwmConfig(CT1Output0_P1_10, 0, 50);
    
    LED_Init();
    KEY_Init();
    UART_InitConfig(USART0, 115200);
#ifdef TFT1_8
    TFTSPI_Init(1);                //TFT1.8初始化  
    TFTSPI_CLS(u16BLUE);           //清屏
    TFTSPI_P8X16Str(3,0,"LQ Motor PWM",u16RED,u16BLUE);
#else
    LCD_Init();
    LCD_CLS();                     //LCD清屏  
    LCD_P8x16Str(13,0,"LQ Servo PWM");
#endif

    char txt[16];
    int servopwm = 750; 
    float time = 0;
```

* 舵机驱动函数

>这里需要注意，PWM的占空比可以随便调，但是舵机的占空比不可以随便调。安装舵机舵盘之前，一定要先调中值（1-2ms之间，可以自行设置，默认1.5ms高电平为中值），也就是给高电平1.5ms的PWM让舵机先归中，再安装舵盘。舵机可以旋转180度，但是装到车上之后，打角会被车的机械结构限制（车的转向角有限，如果舵机打角超出范围，会导致舵机堵转损坏舵机），为了防止舵机打角卡死，在设置舵机PWM时，最好设置限幅。

```c
/**
  * @brief    设置舵机占空比
  *
  * @param    duty          ：占空比*SCT_PWM_MAX   
  *
  * @return   
  *
  * @note      舵机中值 1.5ms高电平的PWM波    限幅在 1.3 - 1.7ms之间
  *
  * @example  PWM_SetServoDuty(700);  //设置占空比百分之  700/SCT_PWM_MAX * 100
  *
  * @date     2019/5/23 星期四
  */
void PWM_SetServoDuty(uint16_t duty)    
{
    if(duty > 850) duty = 850;    //限幅，可自行修改
    if(duty < 650) duty = 650;
    CTIMER_SetDuty(CT1Output0_P1_10, duty);
}

```

* 循环中不断检测按键状态，修改PWM占空比，到达控制舵机的目的。

```c
    while (1)
    {      
        switch(KEY_Read(1))  //检测按键 mode:0单按，mode:1连按
        {
            case 1:
                servopwm -= 10;   
				if(servopwm < 650) servopwm = 650;     //限幅，可自行修改
                PWM_SetServoDuty(servopwm);//刷新servopwm频率
                break;           
            case 2:           
                servopwm = 750;
                PWM_SetServoDuty(servopwm);//刷新servopwm频率
                break;
            case 3:           
                servopwm += 10;
				if(servopwm > 850) servopwm = 850; 
                PWM_SetServoDuty(servopwm);//刷新servopwm频率
                break;
        }
        time=(servopwm)*20.0/10000;
        sprintf(txt,"Time:%4.3fms",time);
#ifdef TFT1_8        
        TFTSPI_P8X16Str(3,3,txt,u16RED,u16BLUE);
#else        
        LCD_P8x16Str(2,3,txt);
#endif  
        printf(txt); 
        
        //LED闪烁
        LED_ColorReverse(red);      
        delayms(50);
    }
```

---
---
---

### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个；
2) LPC54606 迷你 子母板一套+TFT18/OLED一块+舵机+电池；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
2) 打开配套程序，连接下载器；
3) 插上TFT18/OLED，接好舵机；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
按键 K1按下 舵机归中  K0、K2按下，舵机打角

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 

安装舵机舵盘之前，一定要先调中值（1-2ms之间，可以自行设置，默认1.5ms高电平为中值），也就是给高电平1.5ms的PWM让舵机先归中，再安装舵盘。舵机可以旋转180度，但是装到车上之后，打角会被车的机械结构限制（车的转向角有限，如果舵机打角超出范围，会导致舵机堵转损坏舵机），为了防止舵机打角卡死，在设置舵机PWM时，最好设置限幅。