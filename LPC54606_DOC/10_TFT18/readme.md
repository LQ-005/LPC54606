[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# TFT18例程

> TFT彩屏相比OLED，可以显示更多的像素和色彩，但是相对显示会更耗时。

# 目录  

- [硬件电路](##硬件电路)  
- [软件实现](##软件实现)  
  - [TFT18简介](###TFT18简介)  
  - [常用API](###常用API)
  - [程序讲解](###程序讲解)  
  - [下载测试](###下载测试)  
- [注意事项](##注意事项)


## 硬件电路

>>TFT18采用SPI的方式驱动，使用5个IO即可控制。和OLED接口兼容（管脚定义不同）。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB77feb4dbbc2c4663a929dfd125ad8f3f?method=download&shareKey=4b2a4aceb6f161fd6365137a54b12162">
</div>

## 软件实现



### TFT18简介

TFT18可以看作128*160的点阵，每个点可以显示RGB色彩，在TFT18上显示字符、图片、汉字本质上都是控制每个点的状态。ASCII字符数量比较少而且比较常用，所以将ASCII字符集的显示数组存放到LQ_SGP18T.c文件中，方便用户使用，而图片和汉字则需要用户自己制作显示数组。

### 常用API

>>>龙邱的LPC54606资料用户手册文件夹中有一个LPC54606V1.1精简版API帮助手册，里面记录了龙邱上层库函数的帮助信息，用户可以自行查看。

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBa1492b565360cbc4819d4ffdd6c5f0d9?method=download&shareKey=7d5a2745ff9aa655e86587f22890af51">
</div>

* TFT18初始化

TFT18采用SPI方式控制，但是为了方便移植，LQ_SGP18T.c驱动文件内置了模拟SPI协议，可以使用任意IO控制TFT18。

```c
/*!
  * @brief    TFT18初始化
  *
  * @param    type ： 0:横屏  1：竖屏
  *
  * @return   无
  *
  * @note     如果修改管脚 需要修改初始化的管脚 
  *
  * @see      TFTSPI_Init(1);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Init(uint8_t type)
```

---
---
---

* 全屏显示单色画面

>>>为了防止上次显示的内容还未清空，初始化后面最好加上清屏操作

```c
/*!
  * @brief    全屏显示单色画面
  *
  * @param    color ：填充的颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_CLS(u16YELLOW);	
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_CLS(uint16_t color)
```

---
---
---

* 显示字符

>>>LQ_SGP18T.c驱动文件内置了两种ASCII的显示数组，分别支持6x8字体和8x16字体.注意，屏幕左上角坐标为（0,0），右下角（127,160）

```c
/*!
  * @brief    液晶字符串输出(6*8字体)
  *
  * @param    x: 0 - 20 (行)
  * @param    y: 0 - 19 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_P6X8Str(1, 1, "123456", u16YELLOW, u16RED); 
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P6X8Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);

/*!
  * @brief    液晶字符串输出(8*16字体)
  *
  * @param    x: x: 0 -15   (行) 
  * @param    y: y: 0 -9  	 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_P8X16Str(1, 1, "123456", u16YELLOW, u16RED); 
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P8X16Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);
```

---
---
---

* 显示变量

>>>使用TFT18最常用的就是将变量的值显示出来,可以使用sprintf函数将变量填充到字符串中，在通过显示字符串方式将变量显示出来。

```c
/* 使用sprintf函数需要stdio.h头文件支持 */
#include "stdio.h"

char  txt[30];

float count = 1.0;

/* 将变量填充到字符串中 */
sprintf(txt, "variate: %.2f", count);

/* 将字符串显示出来 */
TFTSPI_P8X16Str(0, 6, txt, u16RED, u16BLUE); 

```

---
---
---

* 显示汉字

>>>LQ_SGP18T.c驱动文件内置了一些汉字字库文件。使用TFTSPI_P16x16Str函数显示汉字时，参数s_dat会先和hanzi_Idx[]数组中的汉字进行对比，如果hanzi_Idx[]数组中有s_dat则利用hanzi16x16[]字库中对应的编码将汉字显示出来。如果没有要显示的汉字，则需要自行制作字库。（14*16字库同理）
```c 
/** 简单的汉字字库 */
unsigned char hanzi_Idx[] = 
{     
  "北京龙邱丘智能科技淘宝店液晶单片机智能车器人启动中电压量不足请稍候，！：℃攻击娱乐模式普通格斗对话俯卧撑倒立握手敬礼跳舞蹈运动蓝精灵太极街舞温度—"
};

unsigned char hanzi16x16[] = {	
  0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x44,0x04,0x48,0x7C,0x50,0x04,0x60,0x04,0x40,
  0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x42,0x1C,0x42,0xE4,0x42,0x44,0x3E,0x04,0x00,//北,0
  0x02,0x00,0x01,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x1F,0xF0,0x10,0x10,0x10,0x10,
  0x10,0x10,0x1F,0xF0,0x01,0x00,0x11,0x10,0x11,0x08,0x21,0x04,0x45,0x04,0x02,0x00,//京,1
  //...后面省略 
};


/*!
  * @brief    液晶汉字字符串输出(16*16字体)
  *
  * @param    x: 0 - 7	(行)
  * @param    y: 0 - 9	(列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     汉字只能是字库里的 字库没有的需要自行添加
  *
  * @see      TFTSPI_P16x16Str(1, 1, "123456", u16YELLOW, u16RED); 
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P16x16Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);


/*!
  * @brief    液晶汉字字符串输出(12*16字体)
  *
  * @param    x: 0 - 7	(行)
  * @param    y: 0 -12	(列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     汉字只能是字库里的 字库没有的需要自行添加
  *
  * @see      TFTSPI_P16x12Str(1, 1, "123456", u16YELLOW, u16RED); 
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P16x12Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);
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

* 初始化TFT，显示龙邱LOGO

```c
TFTSPI_Init(0);        //LCD初始化  0:横屏  1：竖屏
TFTSPI_CLS(u16BLUE);   //蓝色屏幕	
TFTSPI_Show_Logo(0,37);//显示龙邱LOG   
TFTSPI_P16x16Str(0,0,"北京龙邱智能科技",u16RED,u16BLUE);		//字符串显示
TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);		//字符串显示 
```

* 在循环中，不断显示

```c
char txt[32];
float count = 1.0;
while(1)
{
    sprintf(txt, "variate: %.2f", count);                     //将变量填充到字符串的对应位置，并将字符串存放到txt[]中
    TFTSPI_P8X16Str(0, 6, txt, u16RED, u16BLUE);              //将txt中 内容显示出来
    lq_tft_delayms(1000);
    count *= (float)1.1;      
}
```

---
---
---

### 下载测试

实验准备：
1) LQ-DAPLINK 下载器一个；
2) LPC54606 迷你 子母板一套+TFT18一块；
3) PC 电脑一套；
4) IAR8.2 及以上版本软件；
5) 龙邱配套例程一份；
试验方法/步骤：
1) 安装 IAR 软件（首次使用需要安装，后续则直接使用即可）；
2) 打开配套程序，连接下载器；
3) 插上TFT18；
4) 下载程序到 LPC54606 迷你 核心板并运行；  

实验结果：
TFT18会显示

## 注意事项

>软件仅供技术交流，如有错误或者建议，可以随时联系 3070171375@qq.com 