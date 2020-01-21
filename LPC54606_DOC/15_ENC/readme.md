[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# ENC例程

>带方向编码器测试例程，LPC54606内部没有正交解码模块，因此需要使用定时器的脉冲计数功能对带方向编码器的脉冲进行计数。定时器一般使用内部时钟源，在内部时钟的上升沿或者下降沿计数，但是在做脉冲计数时使用的是外部时钟源，在特定管脚的上升沿或者下降沿计数。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [CTIME计数简介](###CTIME计数简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)

## 硬件电路

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB2074115f58bbac786a993151c2ade278?method=download&shareKey=35f71e51ccce0daec54f51750809ce30">
</div>

带方向编码器时序图如上所示，除了电源和地，还有三个信号线LSB、DIR、Z ，LSB是输出脉冲，编码器多少线旋转一圈LSB就输出多少个脉冲；DIR是方向信号，顺时针旋转输出高电平，逆时针旋转输出低电平；Z是零位信号，旋转一圈会输出一个脉冲，可以用来检测编码器线数，一般用LSB和DIR即可。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB7d0af3960ff4ad57210286ea3aead8fa?method=download&shareKey=3a60ef9ae2b4aa298a17191eaed2973f">
</div>

LPC54606不带正交解码模块，这里使用定时器的脉冲计数功能，对带方向编码器进行计数，并且读取旋转方向电平信号，判断正反转。（注意编码器4的LSB和ADC2公用一个管脚，两个功能同时只能使用其中一个）

## 软件实现

### CTIME计数简介

> LPC54606内部有5个CTIME模块，每个CTIME都有外部输入管脚，每个CTIME都可以用脉冲计数功能对一个带方向编码器的脉冲进行计数。

### 常用API

>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>

* ENC初始化

CTIME进行脉冲计数功能初始化，一个CTIME有4个通道，可以选择其中一个通道管脚作为脉冲计数的输入管脚。

```c
/** 
  * @brief CTIMER模块 脉冲计数通道
  * @note  一个CTIMER 只能有一个通道用作脉冲计数 一个CTIMER模块只能用作脉冲计数和PWM输出中的一种
  */ 
typedef enum
{
    CT0Input0_P0_1  = 0x0000 + 1,  CT0Input0_P0_13 = 0x0000 + 2,     /*!< CTIMER0 输入通道0管脚  */  
    CT0Input1_P0_14 = 0x0100 + 1,  CT0Input1_P0_2  = 0x0100 + 2,     /*!< CTIMER0 输入通道1管脚  */  
    CT0Input2_P1_0  = 0x0200 + 1,  CT0Input2_P1_28 = 0x0200 + 2,     /*!< CTIMER0 输入通道2管脚  */  
    CT0Input3_P1_1  = 0x0300 + 1,  CT0Input3_P1_26 = 0x0300 + 2,     /*!< CTIMER0 输入通道3管脚  */  
	
    CT1Input0_P1_9  = 0x0400 + 1,  CT1Input0_P0_16 = 0x0400 + 2,     /*!< CTIMER1 输入通道0管脚  */  
    CT1Input1_P1_11 = 0x0500 + 1,                                    /*!< CTIMER1 输入通道1管脚  */  
    CT1Input2_P1_13 = 0x0600 + 1,                                    /*!< CTIMER1 输入通道2管脚  */  
    CT1Input3_P1_15 = 0x0700 + 1,                                    /*!< CTIMER1 输入通道3管脚  */  
	
    CT2Input0_P0_24 = 0x0800 + 1,                                    /*!< CTIMER2 输入通道0管脚  */  
    CT2Input1_P0_25 = 0x0900 + 1,                                    /*!< CTIMER2 输入通道1管脚  */  
    CT2Input2_P0_10 = 0x0A00 + 1,                                    /*!< CTIMER2 输入通道2管脚  */  
    CT2Input3_P0_28 = 0x0B00 + 1,                                    /*!< CTIMER2 输入通道3管脚  */
	                                                                    
    CT3Input0_P0_4 = 0x0C00 + 1,                                     /*!< CTIMER3 输入通道0管脚  */  
    CT3Input1_P0_6 = 0x0D00 + 1,                                     /*!< CTIMER3 输入通道1管脚  */  
    CT3Input2_P1_20= 0x0E00 + 1,  CT3Input2_P0_26= 0x0E00 + 2,       /*!< CTIMER3 输入通道2管脚  */  
    CT3Input3_P0_20= 0x0F00 + 1,  CT3Input3_P0_22= 0x0F00 + 2,       /*!< CTIMER4 输入通道3管脚  */ 
	
    CT4Input0_P0_15= 0x1000 + 1,                                     /*!< CTIMER4 输入通道0管脚  */  
    
} CTIMER_InputChannel_t;

/*!
  * @brief    初始化CTIMER 作为脉冲计数功能
  *
  * @param    base     ： CTIMER0 - CTIMER4
  * @param    channel  ： 输入脉冲计数通道
  *
  * @return   无
  *
  * @note     LPC只能用带方向编码器  
  *
  * @see      CTIMER_InitCapConfig(CTIMER0, CT0Input0_P0_1);//初始化CTIMER0 为脉冲捕获功能 使用P0_1管脚捕获脉冲
  *
  * @date     2019/10/21 星期一
  */
void CTIMER_InitCapConfig(CTIMER_Type *base, CTIMER_InputChannel_t channel);

```

---
---
---

* 获取脉冲计数值

带方向编码器方向管脚需要接一个普通的IO口，读取IO口电平判断方向信息，获取脉冲计数值函数的第二个参数就是方向管脚的管脚标号（注意使用前请先初始化方向管脚），获取脉冲值后该函数会自动清零。
```c
/*!
  * @brief    CTIMER 得到计数值
  *
  * @param    base        ： CTIMER0 - CTIMER4
  * @param    pin         ： 方向管脚 DIR
  *
  * @return   脉冲计数值
  *
  * @note     使用前需要对CTMIE初始化 方向管脚也需要初始化
  *
  * @see      CTIMER_GetCounter(CTIMER0, P1_11); //获取CTIMER0脉冲计数值 P1_11接编码器方向管脚（DIR）
  *
  * @date     2019/5/7 星期二
  */
int16_t CTIMER_GetCounter(CTIMER_Type *base, GPIO_Name_t pin);

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

* 初始化LED（初始化后会亮白灯）、串口、屏幕等模块，初始化编码器方向管脚和定时器脉冲计数功能。

>OLED和TFT18都可以使用，具体使用那个屏显示，请打开config.h，选择对应的显示屏。

```c

/* 注意 LPC54606 只能用带方向编码器 */
#define ENC1_DIR    P1_11          /*!< 编码器1方向 */
#define ENC2_DIR    P1_21          /*!< 编码器2方向 */
#define ENC3_DIR    P1_16          /*!< 编码器3方向 */
#define ENC4_DIR    P1_8           /*!< 编码器4方向 */

    LED_Init();
    UART_InitConfig(USART0,115200);
    
#ifdef OLED
    LCD_Init();
    LCD_CLS();                   //LCD清屏
    LCD_P8x16Str(4,0,"LQ ENC PWM");  
#else
    TFTSPI_Init(1);               //TFT1.8初始化 
    TFTSPI_CLS(u16BLUE);          //清屏  
    TFTSPI_P16x16Str(0,0,"北京龙邱智能科技",u16RED,u16BLUE);
#endif 
    
    printf("带方向编码器测试\n");
    printf("编码器1的脉冲接P0_1   方向接P1_11 \n");
    printf("编码器2的脉冲接P0_24  方向接P1_21 \n");
    printf("编码器3的脉冲接P0_4   方向接P1_16 \n");
    printf("编码器4的脉冲接P0_15  方向接P1_8  \n");
    
    CTIMER_InitCapConfig(CTIMER0, CT0Input0_P0_1);          //用CTIMER0 P0_1作为脉冲输入口
    CTIMER_InitCapConfig(CTIMER2, CT2Input0_P0_24);         //用CTIMER2 P0_24作为脉冲输入口
    CTIMER_InitCapConfig(CTIMER3, CT3Input0_P0_4);          //用CTIMER3 P0_4作为脉冲输入口
    CTIMER_InitCapConfig(CTIMER4, CT4Input0_P0_15);         //用CTIMER4 P0_15作为脉冲输入口 注意会少一个ADC口
    
    /* 方向管脚 */
    PIN_InitConfig(ENC1_DIR, PIN_MODE_INPUT_PULLUP, 1);
    PIN_InitConfig(ENC2_DIR, PIN_MODE_INPUT_PULLUP, 1);
    PIN_InitConfig(ENC3_DIR, PIN_MODE_INPUT_PULLUP, 1);
    PIN_InitConfig(ENC4_DIR, PIN_MODE_INPUT_PULLUP, 1);

    char txt[16];
    int speed1, speed2, speed3, speed4;
```

* 循环中不断读取脉冲值，并显示在屏幕上

>市面上的编码器，都会说是多少线的，比如512线编码器意思就是编码器旋转一圈产生512个脉冲，通过ENC可以读取到数值512。这个值和小车的速度是线性关系的，一般情况下可以不去换算具体速度直接使用即可。如果想要换算成具体速度，则需要知道小车轮子周长X0、小车齿轮X1和编码器齿轮的数目X2、编码器的线数X3、ENC读到数据X4和ENC读取时间间隔X5。速度V = ((X4/X3) * X2 / X1) * X0 / X5。

```c
    while(1)
    {

        /* 获取 编码器计数值 */
        speed1 = CTIMER_GetCounter(CTIMER0, ENC1_DIR);
        
        /* 获取 编码器计数值 */
        speed2 = CTIMER_GetCounter(CTIMER2, ENC2_DIR);
        
        /* 获取 编码器计数值 */
        speed3 = CTIMER_GetCounter(CTIMER3, ENC3_DIR);
          
        /* 获取 编码器计数值 */
        speed4 = CTIMER_GetCounter(CTIMER4, ENC4_DIR);
            
#ifdef OLED
            printf("\r\n/ENC1 %5d \r\n ",speed1);
            sprintf(txt,"enc1:%5d ",speed1);
            LCD_P6x8Str(20,2,txt);
            
            printf("\r\n/ENC2 %5d \r\n ",speed2);
            sprintf(txt,"enc2:%5d ",speed2);
            LCD_P6x8Str(20,3,txt);
            
            printf("\r\n/ENC3 %5d \r\n ",speed3);
            sprintf(txt,"enc3:%5d ",speed3);
            LCD_P6x8Str(20,4,txt);
            
            printf("\r\n/ENC4 %5d \r\n ",speed4);
            sprintf(txt,"enc4:%5d ",speed4);
            LCD_P6x8Str(20,5,txt);
#else
            printf("\r\n/ENC1 %5d \r\n ",speed1);
            sprintf(txt,"enc1:%5d ",speed1);
            TFTSPI_P8X16Str(2, 3, txt, u16RED, u16BLUE);
            
            printf("\r\n/ENC2 %5d \r\n ",speed2);
            sprintf(txt,"enc2:%5d ",speed2);
            TFTSPI_P8X16Str(2, 4, txt, u16RED, u16BLUE);
            
            printf("\r\n/ENC3 %5d \r\n ",speed3);
            sprintf(txt,"enc3:%5d ",speed3);
            TFTSPI_P8X16Str(2, 5, txt, u16RED, u16BLUE);
            
            printf("\r\n/ENC4 %5d \r\n ",speed4);
            sprintf(txt,"enc4:%5d ",speed4);
            TFTSPI_P8X16Str(2, 6, txt, u16RED, u16BLUE);
#endif            
            delayms(100);
            LED_ColorReverse(green);
    }
```

---
---
---

### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个；
2) LPC54606 迷你 子母板一套+TFT18/OLED一块+带方向编码器+电池；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
2) 打开配套程序，连接下载器；
3) 插上TFT18/OLED，接好编码器；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
转动带方向编码器，屏幕上显示脉冲值。

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 

注意LPC54606只能用带方向编码器