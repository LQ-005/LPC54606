/*!
  * @file     main.c
  *
  * @brief    
  *
  * @company  北京龙邱智能科技
  *
  * @author   LQ-005
  *
  * @note     
  *
  * @version  V1.0
  *
  * @Software IAR8.22 or MDK5.25
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2019/10/19 星期六
  */ 
#include "include.h"

/* 中断优先级组 */
#define NVIC_Group0   0x07
#define NVIC_Group1   0x06
#define NVIC_Group2   0x05
#define NVIC_Group3   0x04


/**
  * @brief    不精确延时
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @example  delayms(100);
  *
  * @date     2019/4/22 星期一
*/
void delayms(uint16_t ms)
{
	volatile uint32_t i = 0;
	while(ms--)
	{
		for (i = 0; i < 15000; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}	
}


/*!
 * @brief Main function
 */
int main(void)
{
    /* 初始化板子时钟 180M */
    BOARD_BootClockPLL180M();
    
    /* 初始化串口0 可以使用 PRINTF函数 */
    BOARD_InitDebugConsole();

    
    /*设置中断优先级组  0: 0个抢占优先级 8个子优先级 
     *                  1: 2个抢占优先级 4个子优先级 
	 *		            2: 4个抢占优先级 2个子优先级 
     *                  3: 8个抢占优先级 0个子优先级 
     */
	/* 配置优先级分组 3: 8个抢占优先级 0-7  0优先级最高 */
    NVIC_SetPriorityGrouping(NVIC_Group3);
    
    /*!
      * @brief   测试串口
      *
      * @note    串口0和串口3打印字符串 并且会打印接收到的字符
      * @note    如果使用龙邱的DAPlink，连接上核心板后 设置串口助手波特率115200  8位数据位 1位停止位 没有奇偶校验 打开对应COM口，即可看到打印信息  
      * @note    如果使用龙邱的串口蓝牙模块，蓝牙波特率和核心板串口波特率设置一致，P0_30接RX  P0_29接TX  GND接上
      * @note    如果使用USB转TTL   GND相连  TX接RX  RX接TX  波特率一样
      * @note    P0_29 ---- 单片机串口0 RX  可以在LQ_UART.h中通过宏定义修改管脚
      * @note    P0_30 ---- 单片机串口1 TX
      * @note    P0_3  ---- 单片机串口3 RX  可以在LQ_UART.h中通过宏定义修改管脚
      * @note    P0_2  ---- 单片机串口3 TX
      * @note    实现现象  打印 北京龙邱智能科技. 并且打印接收到的字符
      */ 
    Test_UART();
    
    while(1)
    {
    
    }
}
