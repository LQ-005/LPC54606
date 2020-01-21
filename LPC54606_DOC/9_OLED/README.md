[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# OLED例程

> OLED是一种比较常见的人机交互界面设备。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [OLED简介](###OLED简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)


## 硬件电路

>>OLED采用SPI的方式驱动，使用四个IO即可控制，CS管脚可以不接（默认拉低选中的）。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB77feb4dbbc2c4663a929dfd125ad8f3f?method=download&shareKey=4b2a4aceb6f161fd6365137a54b12162">
</div>

## 软件实现

>>为了和以前的lcd兼容，函数命名都是LCD开头的。

### OLED简介

OLED可以看作128*64的点阵，每个点有亮灭两种状态，在OLED上显示字符、图片、汉字本质上都是控制每个点的状态。ASCII字符数量比较少而且比较常用，所以将ASCII字符集的显示数组存放到LQ_12864SPI.c文件中，方便用户使用，而图片和汉字则需要用户自己制作显示数组。

### 常用API

>>>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>


* OLED初始化

OLED采用SPI方式控制，但是为了方便移植，LQ_12864SPI.c驱动文件内置了模拟SPI协议，可以使用任意四个IO控制OLED。

```c
/*!
  * @brief    OLED初始化
  *
  * @param    无
  *
  * @return   无
  *
  * @note     如果修改管脚 需要修改初始化的管脚 
  *
  * @see      LCD_Init();
  *
  * @date     2019/6/13 星期四
  */
void LCD_Init(void)  
```

---
---
---

* 清屏

>>>为了防止上次显示的内容还未清空，初始化后面最好加上清屏操作

```c
/*!
  * @brief    OLED清屏
  *
  * @param    无  
  *
  * @return   无
  *
  * @note     无
  *
  * @see      LCD_CLS(); //清屏
  *
  * @date     2019/6/13 星期四
  */
void LCD_CLS(void);
```

---
---
---

* 显示字符

>>>LQ_12864SPI.c驱动文件内置了两种ASCII的显示数组，分别支持6x8字体和8x16字体.注意，屏幕左上角坐标为（0,0），右下角（127,63）

```c
/*!
  * @brief    写入一组标准ASCII字符串
  *
  * @param    x  : 0-127   
  * @param    y  : 0-7
  * @param    ch : 要显示的字符串
  *
  * @return   无
  *
  * @note     无
  *
  * @see      LCD_P6x8Str(0,0,"OLED 3.");  //显示字符串
  *
  * @date     2019/6/13 星期四
  */
void LCD_P6x8Str(unsigned char x,unsigned char y, char ch[]);

/*!
  * @brief    写入一组标准ASCII字符串
  *
  * @param    x  : 0-127   
  * @param    y  : 0-6
  * @param    ch : 要显示的字符串
  *
  * @return   无
  *
  * @note     一个字符高16  OLED高度64  
  *
  * @see      LCD_P8x16Str(0,0,"OLED 3.");  //显示字符串
  *
  * @date     2019/6/13 星期四
  */
void LCD_P8x16Str(unsigned char x,unsigned char y, char ch[]);
```

---
---
---

* 显示变量

>>>使用OLED最常用的就是将变量的值显示出来,可以使用sprintf函数将变量填充到字符串中，在通过显示字符串方式将变量显示出来。

```c
/* 使用sprintf函数需要stdio.h头文件支持 */
#include "stdio.h"

char  txt[30];

int6_t aacx;

/* 将变量填充到字符串中 */
sprintf((char*)txt,"ax:%06d",aacx);

/* 将字符串显示出来 */
LCD_P6x8Str(0,2,txt);

```

---
---
---

* 显示汉字

>>>LQ_12864SPI.c驱动文件内置了一些汉字字库文件。使用LCD_P14x16Str函数显示汉字时，参数ch[]会先和F14x16_Idx[]数组中的汉字进行对比，如果F14x16_Idx[]数组中有ch[]则利用F14x16[]字库中对应的编码将汉字显示出来。如果没有要显示的汉字，则需要自行制作字库。
```c 
/** 简单的汉字字库 */
const uint8_t F14x16_Idx[] = 
{
    "北京电子科技职业学院龙邱智能机器人遥控器接收发送按键："
};

/** 14*16 简单的汉字字库*/
const uint8_t F14x16[] = {  
    0x20,0x20,0x20,0x20,0xFE,0x00,0x00,0x00,0xFE,0x80,0x40,0x20,0x10,0x00,
    0x10,0x10,0x08,0x04,0x7F,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x70,0x00,//北0
    0x08,0x08,0xE8,0x28,0x28,0x2A,0x2C,0x28,0x28,0x28,0xE8,0x08,0x08,0x00,
    0x00,0x20,0x13,0x0A,0x42,0x42,0x7E,0x02,0x02,0x0A,0x13,0x20,0x00,0x00,//京1
    //....后面的省略
};

/*!
  * @brief    输出汉字字符串
  *
  * @param    x  : 0-127   
  * @param    y  : 0-6
  * @param    ch : 要显示的字符串
  *
  * @return   无
  *
  * @note     一个字符高16  OLED高度64   字库需要自己制作
  *
  * @see      LCD_P14x16Str(0,4,"北京龙邱");   //显示汉字
  *
  * @date     2019/6/13 星期四
  */
void LCD_P14x16Str(unsigned char x,unsigned char y, char ch[]);

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

* 初始化OLED，显示龙邱LOGO，清屏

```c
    LCD_Init();                          //LCD初始化 
    LCD_Show_LQLogo();                   //显示LOGO
    lq_lcd_delayms(2000);  
    LCD_CLS();
```

* 在循环中，不断显示

```c
while (1)
    {  
        LCD_P6x8Str(0,7,"OLED 3.");         //字符串
		lq_lcd_delayms(1000);
        LCD_Rectangle(0, 0, 127,62,0);      //绘制矩形
        LCD_Rectangle(24,8,88,55,1);
		LCD_Rectangle(32,16,80,47,1);
		LCD_Rectangle(40,24,72,39,1);
        lq_lcd_delayms(1000);
        LCD_CLS();                          //清屏
        
        LCD_P8x16Str(0, 0,"OLED 4.");       //显示字符串
        LCD_P14x16Str(0,4,"北京龙邱");    //显示汉字
        LCD_Print(0,6,"北京龙邱 OLED.");  //显示汉字+字符
        LCD_PutPixel(120,60);
        lq_lcd_delayms(1000);
        LCD_CLS();                          //清屏
        
        LCD_Fill();                         //填充
        lq_lcd_delayms(1000);
        LCD_CLS();                          //清屏 
        
        //延时
        lq_lcd_delayms(50);
    }
```
---
---
---

### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个；
2) LPC54606 迷你 子母板一套+OLED一块；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
2) 打开配套程序，连接下载器；
3) 插上OLED；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
OLED会显示

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 