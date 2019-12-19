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
      * @brief   测试电机 可以用示波器观察波形  
      *          注意一个电机由两路PWM控制，一路PWM占空比给0，一路给PWM信号（PWM信号占空比越大，电机驱动的输出电压越大，电机扭矩越大）
      *          电机反转，将两路PWM信号对调即可。电机驱动上的排针除了PWM之外 一般会有一个电源V插针，这个是给驱动上的光耦供电的，接3.3V即可
      *          1：电机1： 使用 P0_26  P1_17 
      *          2：电机2： 使用 P0_28  P0_27  
      *          3：电机3： 使用 P0_19  P0_22  
      *          4：电机4： 使用 P0_18  P0_17  
      *
      * @note    按键 K1按下 电机停止  K0、K2按下，电机加减速
      */ 
    TEST_MOTOR();
    
    while(1)
    {
    
    }
}
