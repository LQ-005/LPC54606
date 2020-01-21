[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# MOTOR例程

>电机驱动例程，电机驱动简单来讲就是控制PWM占空比，从而调节电机驱动输出电压，控制电机转速。

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
<img src="https://note.youdao.com/yws/api/personal/file/WEBbdfdf4ad0c27ed4b60079b04b8684cb5?method=download&shareKey=4541dd00e6ac418fabc814123846bc03">
</div>

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB8f54357137f5afc3935722309fe9604d?method=download&shareKey=ab5c7c43e68bda16ac630b7af670487e">
</div>

接线说明：电池正极接母板电源接口正极和电机驱动板电源接口正极，电池负极接母板电源接口负极和电机驱动板电源接口负极（注意不要接反，电机驱动电流比较大，需要用比较粗电源线接，不要用杜邦线），自己想办法接上去，接头什么的都可以。 

电机不分正负，接在电机驱动的电机端子上即可，如果电机转的方向不对可以调换电机接线。  

母板上的3.3V接电机驱动光耦供电PIN，母板4路PWM信号按顺序接电机驱动板上的4路PWM信号。

整个流程就是单片机输出PWM ---> PWM控制电机驱动的输出电压影响电机转速。

## 软件实现

### PWM简介

> PWM是单片机内部定时器产生的，龙邱LPC54606VD母板上使用的电机PWM是由SCTIME产生的，SCTIME可以输出8路频率相同的PWM，可以用于控制4路电机。

### 常用API

>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>

* PWM初始化

SCT输出PWM的初始化函数需要三个参数，第一个是输出通道（每个通道只能使用其中一个管脚）；第二个参数时输出PWM频率，电机一般使用10-20Khz，注意SCT输出的所有PWM只能是同一频率的；第三个数占空比参数，占空比一般情况下是0-100%，但是这样粒度太大了，不方便控制，因此使用宏定义SCT_PWM_MAX，定义占空比最大值，默认是10000,当参数duty是1000时，实际占空比是1000/SCT_PWM_MAX * 100；

```c

#define SCT_PWM_MAX                  10000     /*!< PWM占空比输出最大值 */


/** 
  * @brief SCT0模块 输出通道 
  */
typedef enum  
{
    //SCT0通道     管脚  可选管脚 完整2019      
    SctOutPut0_P0_2 = 0x0000 + 1, SctOutPut0_P0_17= 0x0000 + 2, SctOutPut0_P1_4 = 0x0000 + 3, SctOutPut0_P1_23= 0x0000 + 4, 
    SctOutPut1_P0_3 = 0x0100 + 1, SctOutPut1_P0_18= 0x0100 + 2, SctOutPut1_P1_8 = 0x0100 + 3, SctOutPut1_P1_24= 0x0100 + 4, 
    SctOutPut2_P0_15= 0x0200 + 1, SctOutPut2_P0_19= 0x0200 + 2, SctOutPut2_P1_9 = 0x0200 + 3, SctOutPut2_P1_25= 0x0200 + 4, 
    SctOutPut3_P0_22= 0x0300 + 1, SctOutPut3_P0_31= 0x0300 + 2, SctOutPut3_P1_10= 0x0300 + 3, SctOutPut3_P1_26= 0x0300 + 4, 
    SctOutPut4_P0_23= 0x0400 + 1, SctOutPut4_P1_3 = 0x0400 + 2, SctOutPut4_P1_17= 0x0400 + 3, 
    SctOutPut5_P0_26= 0x0500 + 1, SctOutPut5_P1_18= 0x0500 + 2, 
    SctOutPut6_P0_27= 0x0600 + 1, SctOutPut6_P1_31= 0x0600 + 2,
    SctOutPut7_P0_28= 0x0700 + 1, SctOutPut7_P1_19= 0x0700 + 2,
    SctOutPut8_P0_29= 0x0800 + 1, 
    SctOutPut9_P0_30= 0x0900 + 1, 
} SCT0_OutChnannel_t;

/*!
  * @brief    SCT 输出PWM初始化
  *
  * @param    channel    ： SCT通道  LQ_SCTIME.h 中可的枚举体
  * @param    frequency  ： 频率  注意SCT输出的PWM频率必须一样
  * @param    duty       ： 占空比百分之 duty/SCT_PWM_MAX *100  
  *
  * @return   无
  *
  * @note     SCT最多10个事件，其中一个事件用于PWM周期，其他九个事件可以产生9路频率相同的PWM
  * @note     但是摄像头也需要用到SCT的事件，如果使用神眼带与非门的转接座（白色），需要一个输入事件接摄像头像素同步信号触发DMA，最多就只能输出8路PWM
  * @note     如果使用神眼老板转接座或者OV7725，就需要更多的输入事件 不可以驱动8路PWM了。
  *
  * @see      SCT_InitPwmConfig(SctOutPut0_P0_2, 18000, 500); //设置通道0 P0_2输出频率 18000 占空比百分之 500/SCT_PWM_MAX *100
  *
  * @date     2019/10/21 星期一
  */
void SCT_InitPwmConfig(SCT0_OutChnannel_t channel, uint32_t frequency, uint32_t duty)
```

---
---
---

* 设置占空比

```c
/**
  * @brief    SCT 设置PWM占空比
  *
  * @param    channel    ： SCT通道  LQ_SCTIME.h 中的枚举体
  * @param    duty       ： 占空比百分之 duty/SCT_PWM_MAX *100
  *
  * @return   无
  *
  * @note     使用前需要初始化
  *
  * @example  SCT_PwmSetDuty(SctOutPut0_P0_2, 500);  //设置通道0 P0_2输出占空比百分之 500/SCT_PWM_MAX *100
  *
  * @date     2019/5/8 星期三
  */
void SCT_PwmSetDuty(SCT0_OutChnannel_t channel, uint32_t duty);
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
     /* 初始化PWM */
    SCT_InitPwmConfig(SctOutPut0_P0_17, 18000, 0);
    SCT_InitPwmConfig(SctOutPut1_P0_18, 18000, 0);
    SCT_InitPwmConfig(SctOutPut2_P0_19, 18000, 0);
    SCT_InitPwmConfig(SctOutPut3_P0_22, 18000, 0);
    SCT_InitPwmConfig(SctOutPut4_P1_17, 18000, 0);
    SCT_InitPwmConfig(SctOutPut5_P0_26, 18000, 0);
    SCT_InitPwmConfig(SctOutPut6_P0_27, 18000, 0);
    SCT_InitPwmConfig(SctOutPut7_P0_28, 18000, 0);
    
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
    short motorpwm=0;   
```

* 电机驱动函数

>两路PWM驱动一路电机，正常情况下，一路PWM占空比给0,另一路PWM占空比和电机转速成正比；如果想要控制电机反转，两路PWM信号调换即可。

```c
/*!
  * @brief    电机控制函数
  *
  * @param    num： 1：电机1： 使用 P0_26  P1_17   
                    2：电机2： 使用 P0_28  P0_27  
                    3：电机3： 使用 P0_19  P0_22   
                    4：电机4： 使用 P0_18  P0_17
  * @param    duty: 占空比  范围：-SCT_PWM_MAX  ---  SCT_PWM_MAX   
  *
  * @return   
  *
  * @note     电机不分正负
  *
  * @see      PWM_SetMotorDuty(1, -2000);  //电机1 反转 设置占空比百分之 2000/SCT_PWM_MAX * 100
  *
  * @date     2019/10/31 星期四
  */
void PWM_SetMotorDuty(uint8_t num, short duty)
```

* 循环中不断检测按键状态，修改PWM占空比，到达控制电机转速的目的。

```c
    while (1)
    {        
        switch(KEY_Read(1))  //检测按键
        {
            case 0:
                break;
            case 1:
                motorpwm -= 100;
                if(motorpwm < -10000) motorpwm = -10000;
                PWM_SetMotorDuty(1, motorpwm);
                PWM_SetMotorDuty(2, motorpwm);
                PWM_SetMotorDuty(3, motorpwm);
                PWM_SetMotorDuty(4, motorpwm);
                break;           
            case 3:           
                motorpwm += 100;
                if(motorpwm > 10000) motorpwm = 10000;
                PWM_SetMotorDuty(1, motorpwm);
                PWM_SetMotorDuty(2, motorpwm);
                PWM_SetMotorDuty(3, motorpwm);
                PWM_SetMotorDuty(4, motorpwm);
                break;
            case 2:
                motorpwm = 0;
                PWM_SetMotorDuty(1, motorpwm);
                PWM_SetMotorDuty(2, motorpwm);
                PWM_SetMotorDuty(3, motorpwm);
                PWM_SetMotorDuty(4, motorpwm);
            break;
        }
        
        sprintf(txt,"PWM: %4.2f %%",motorpwm/100.0f);
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
2) LPC54606 迷你 子母板一套+TFT18/OLED一块+电机驱动+电机+电池；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
2) 打开配套程序，连接下载器；
3) 插上TFT18/OLED，接好电机驱动和电机；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
按键 K1按下 电机停止  K0、K2按下，电机加减速

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 
