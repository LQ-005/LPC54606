[北京龙邱智能科技](http://shop36265907.taobao.com)

![北京龙邱智能科技](https://note.youdao.com/yws/api/personal/file/WEB8a51a3385904ba8d4505271e9e9be364?method=download&shareKey=c0de97cb64246fe4120904147dbaf057 "北京龙邱智能科技")

# [LPC54606](https://item.taobao.com/item.htm?spm=a1z10.3-c-s.w4002-15882386800.11.64ac136anCC3Z9&id=559640729464)

>>北京龙邱智能科技LPC54606开源资料  
>>本资料适用于龙邱科技LPC54606VD子母板套件

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEB473b043f5710313005c0b5cbf749fe0c?method=download&shareKey=daf74d3cfaebf5d3c706d2d836942a83">
</div>

## LPC54606J512BD资源

* Arm Cortex-M4处理器，180MHz的频率运行。

* 浮点单元（FPU）和内存保护单元（MPU）

* 512 KB的片上闪存程序存储器

* I＆D总线上的总SRAM高达200 KB，包括160 KB连续的主SRAM和一个额外的32 KB SRAM。 8 KB SRAM库，用于USB通信。

* 16 KB的EEPROM。

* Flexcomm接口包含十个串行外围设备（每个Flexcomm接口都可以通过软件选择为USART，SPI或I2C接口）。

* 两个带有专用DMA控制器的CAN FD模块。

* 五个32位通用定时器/计数器

* 一个SCTimer / PWM具有八个输入和十个输出功能（包括捕获和匹配）。

## [LPC54606VD母板资源](https://item.taobao.com/item.htm?spm=a1z10.3-c-s.w4002-15882386800.11.64ac136anCC3Z9&id=559640729464)

<div align="center">
<img src="https://note.youdao.com/yws/api/personal/file/WEBcd448895e83dc561fab92f1357740239?method=download&shareKey=1df369633dd41f8ed66a885ac6dfe736">
</div>

## LPC54606资料文件夹说明

    ├─LPC54606VD                 资料文件夹  
    │  │  readme.md  
    │  │    
    │  └─龙邱LPC54606VD          资料文件夹  
    │      ├─1_产品详情           
    │      │      LPC54606VD.jpg  
    │      │      母板资源介绍.jpg  
    │      │        
    │      ├─2_用户手册、原理图  
    │      │  ├─原理图  
    │      │  │      LQ_546P100MBVD核心板原理图  20191024.pdf  
    │      │  │      LQ_546P100MBVD母板原理图20191024 .pdf  
    │      │  │      
    │      │  ├─封装  
    │      │  │      龙邱LPC54606迷你核心板封装库20191024.rar  
    │      │  │        
    │      │  └─用户手册  
    │      │          LPC54606V1.0精简版.chm                 API帮助文档     
    │      │          LPC546XX.pdf  
    │      │          LPC546XXFAMFS.pdf  
    │      │          UM10912.pdf  
    │      │           
    │      ├─3_Example                                       工程文件  
    │      │  │  IAR不能跳转.png  
    │      │  │  LPC常见问题.pdf  
    │      │  │  MDK如果不能gotodef 请勾选这里并重新编译.png  
    │      │  │  例程说明.txt  
    │      │  │  删除编译连接文件.bat  
    │      │  │  更新说明.txt  
    │      │  │  生成文件目录.bat  
    │      │  │  龙邱LPC54606 IO分配.txt  
    │      │  │    
    │      │  ├─CMSIS            ARM的CMSIS库 包涵一些内核头文件等  
    │      │  │           
    │      │  ├─Drivers          芯片的驱动文件  
    │      │  │  │  
    │      │  │  ├─board         从官方SDK中保留下来的东西 包括时钟初始化 Debug串口初始化等  
    │      │  │  │      
    │      │  │  ├─drivers          底层驱动库  
    │      │  │  │  ├─FSL_LPC54606  官方FSL底层驱动文件    
    │      │  │  │  │      
    │      │  │  │  └─LQ_LPC54606   龙邱基于官方FSL库的上层驱动文件  
    │      │  │  │      │    
    │      │  │  │      ├─peripheral基于龙邱的上层驱动文件 一些外设模块库  
    │      │  │  │      │      
    │      │  │  │      └─test      基于外设模块库 做的测试文件  
    │      │  │  │              
    │      │  │  ├─startup          启动汇编文件  
    │      │  │  │  ├─arm           MDK使用的汇编文件  
    │      │  │  │  │       
    │      │  │  │  ├─gcc           GCC使用的汇编文件  
    │      │  │  │  │      
    │      │  │  │  └─iar           IAR使用的汇编文件  
    │      │  │  │          
    │      │  │  └─utilities        从官方SDK中保留下来的东西 Debug控制台等内容  
    │      │  │          
    │      │  └─Example             例子工程 龙邱的demo都是基于同一套底层文  
    │      │      │          
    │      │      ├─1_LED           LED例子工程  
    │      │      │  │  
    │      │      │  ├─iar  
    │      │      │  │      
    │      │      │  └─mdk  
    │      │                  
    │      ├─4_开发环境及工具  
    │      │          
    │      └─5_附送资料  
    │              SDK_2.6.0_LPC54606J512.zip  
    │              SDK_2.6.0_LPC54606J512_doc.zip  
    │              
    └─LPC54606_DOC             说明文件夹    
            readme.md  
---
---
---

**工程内文件夹说明**
>├─board                          从官方SDK中保留下来的东西  包括时钟初始化  
>├─doc                            说明文档  
>├─drivers                        官方底层fsl库文件  
>├─lpc54606                       龙邱基于官方FSL库的上层驱动文件   
>├─peripheral                     基于龙邱的上层驱动文件 一些外设模块库   
>├─source                         主函数（不同例程，一般只有这里不一样）   
>├─startup                        启动文件  
>├─test                           基于外设模块库 做的测试文件     
>├─uitlities                      从官方SDK中保留下来的东西  Debug控制台等内容   
>├─Output                         IAR生成的.map文件，包涵许多重要信息  


## LPC54606例程讲解
>注意 所有例程公用一套底层库文件，所以为了防止意外更改底层库，将底层库文件属性设置为只读，如果想要修改，将文件属性设置只读去掉即可。
* [1.LED灯测试例程](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/1_LED)
* [2.按键测试例程](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/2_KEY)
* [3.按键外部中断测试例程](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/3_EXTI)
* [4.MRT定时器中断测试例程](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/4_MRT) MRT定时器最大90ms
* [5.RIT定时器中断测试例程](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/5_RIT)
* [6.串口 打印测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/6_UART)
* [7.串口 printf 打印测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/7_printf)
* [8.systick 系统计时器测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/8_SYSTICK)
* [9.oled 显示测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/9_OLED)
* [10.tft1.8 寸屏 显示测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/10_TFT18)
* [11.adc 采集测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/11_ADC)
* [12.看门狗测试例程](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/12_WDOG)
* [13.4路电机测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/13_MOTOR)
* [14.舵机测试 SD5/S3010均可使用](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/14_SERVO)
* [15.编码器读取测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/15_ENC)
* [16.MPU6050/ICM20602  IIC读取原始数据测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/16_MPU6050-ICM20602)
* [17.九轴 IIC 读取原始数据测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/17_9AX)
* [18.VL53 激光测距模块测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/18_VL53)
* [19.超声波测距模块测试](https://gitee.com/LQ-RDC/LPC54606/tree/master/LPC54606_DOC/19_HCSR04)
* 20.ICM20602 SPI方式读取原始数据
* 21.EEPROM 掉电保存数据测试
* 22.串口+DMA收发测试 
* 23.匿名上位机通信测试
* 24.互补融合滤波测试
* 25.卡尔曼滤波测试
* 26.四元数姿态解算测试
* 27.神眼白色转接座 OLED显示
* 28.神眼白色转接座 TFT18显示

## LPC54606常见问题

1) LPC54606 的SCT有十个事件 一个作为周期事件，一个作为摄像头像素同步事件，剩下8个可以作为PWM输出
因此如果用H车 只能使用神眼+新版转接座（白色带与非门版本）

2) LPC54606 可以使用串口或者USB直接下载程序, 但是一般都是使用下载器直接下载方便调试
使用串口、USB下载时，需要设置ISP
ISP模式由3个管脚控制:   

ISP管脚（上电时默认是高电平） | P0_4 | P0_5 | P0_6
--|--|--|--
从FLASH启动（正常启动）| 高电平|  高电平|  高电平
等待串口数据（串口下载）| 低电平|  高电平|  高电平  

...
ISP有7种模式，具体可以看芯片手册第三章 Boot process
有时候上电，程序不跑，可以检查一下是不是使用了ISP管脚， 并且上电时有管脚被拉低了。
解决方法，可以烧录一次关闭ISP例程 永久关闭ISP

## 母板IO分配

    SWD下载口                                                                
    SWCLK       P0_11                                                        
    SWDIO       P0_12                                                        
                                                                            
                                                                            
    调试串口0                                                                
    USART0_RX   P0_29                                                        
    USART0_TX   P0_30                                                        
                                                                            
                                                                            
    LED灯   核心板上的RGB灯                                                  
    LED_R       P0_8                                                         
    LED_G       P0_9                                                         
    LED_B       P1_12                                                        
                                                                            
                                                                            
                                                                            
                                                                            
    摄像头 神眼+新版转接座（白色带与非门版本）                               
    行中断       不使用                                                      
    场中断       VSY        P0_0                                             
    像素同步时钟 PCLK       P1_5                                             
    SCCB通讯     SDA        P1_2                                             
    SCCB通讯     SCK        P1_3                                             
    数据口       D9         p1_31                                            
    数据口       D8         P1_30                                            
    数据口       D7         P1_29                                            
    数据口       D6         P1_28                                            
    数据口       D5         P1_27                                            
    数据口       D4         P1_26                                            
    数据口       D3         P1_25                                            
    数据口       D2         P1_24                                            
                                                                            
                                                                            
                                                                            
    ADC接口  编码器接口4和一路ADC复用（二选一）                              
    AD通道0      P0_10                                                       
    AD通道3      P0_15(注意，这个IO和母版上的编码器接口4复用)                
    AD通道4      P0_16                                                       
    AD通道5      P0_23                                                       
    AD通道6      P1_0                                                        
    AD通道11     P0_31                                                       
                                                                            
                                                                            
                                                                            
    电机PWM接口 使用的SCT可以产生8路频率相同的PWM                            
    PWM1         P0_26                                                       
    PWM2         P1_17                                                       
    PWM3         P0_28                                                       
    PWM4         p0_27                                                       
    PWM5         P0_17                                                       
    PWM6         P0_18                                                       
    PWM7         P0_19                                                       
    PWM8         P0_22                                                       
                                                                            
                                                                            
                                                                            
                                                                            
    舵机PWM      使用CTIME1 产生PWM                                          
    舵机PWM       P1_10                                                      
                                                                            
                                                                            
                                                                            
                                                                            
    编码器       使用CTIME  脉冲捕获 需要使用带方向编码器                    
    编码器1                                                                  
    脉冲引脚     LSB        P0_1                                             
    方向引脚     DIR        P1_11                                            
                                                                            
    编码器2                                                                  
    脉冲引脚     LSB        P0_24                                            
    方向引脚     DIR        P1_21                                            
                                                                            
    编码器3                                                                  
    脉冲引脚     LSB        P0_4                                             
    方向引脚     DIR        P1_16                                            
                                                                            
    编码器4      会占用一个ADC口                                             
    脉冲引脚     LSB        P0_15                                            
    方向引脚     DIR        P1_8                                             
                                                                            
                                                                            
    TFT18/OLED     显示屏                                                    
    CS/DC           P1_7                                                     
    SCL/RST         P1_20                                                    
    SDI/DI          P1_19                                                    
    DC/CK           P1_6                                                     
    RST/            P1_18                                                    
                                                                            
                                                                            
    陀螺仪    IIC                                                            
    SCL             P0_2                                                     
    SDA             p0_3                                                     
                                                                            
    陀螺仪    SPI                                                            
    SPI3_SCK_PIN        P0_6                                                 
    SPI3_MOSI_PIN       P0_3                                                 
    SPI3_MISO_PIN       P0_2                                                 
    SPI3_CS0_PIN        P0_20                                                
                                                                            
                                                                            
                                                                            
    母板按键                                                                 
    KEY0            P0_21                                                    
    KEY1            P0_5                                                     
    KEY2            P1_14                                                    
                                                                            
                                                                            
    母板LED                                                                  
    LED_D0          P1_15                                                    
    LED_D1          P1_4                                                     
                                                                            
                                                                            
    母板拨码开关                                                             
    P0_25                                                                    
    P1_22                                                                    
                                                                            
                                                                            
    干簧管    相当于一个开关 当遇到磁铁时开关会闭合                          
    P1_9    