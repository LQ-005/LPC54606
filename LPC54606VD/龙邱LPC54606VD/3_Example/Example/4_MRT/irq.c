/*!
  * @file     irq.c
  *
  * @brief    中断服务函数
  *
  * @company  北京龙邱智能科技
  *
  * @author   LQ-005
  *
  * @note     中断服务函数 不知道名字可以去startup_LPC54606.s里面找
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

/*!
  * @brief    HardFault中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/11/4 星期一
  */
void HardFault_Handler(void)
{
    PRINTF("进入这里，一般都是数组越界或者操作了未初始化的外设\r\n");
    PRINTF("在程序开始阶段，可以关闭全局中断 DisableInterrupts  当所有外设都初始化完成后再开启全局中断 EnableInterrupts\r\n");
    PRINTF("避免出现中断服务函数中操作未初始化外设\r\n");
    PRINTF("例如初始化了MRT定时器 在MRT中断中使用串口1打印，但是MRT先初始化，进入中断时串口1还未初始化，就导致了操作未初始化的串口1，就会进入这里\r\n");
    while(1);
}


/*!
  * @brief    MRT中断服务函数
  *
  * @param    无
  *
  * @return   
  *
  * @note     Test_MRT()函数使用
  *
  * @see      
  *
  * @date     2019/10/31 星期四
  */
void MRT0_IRQHandler(void)
{
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_0) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_0, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */
        PT0_26 = !PT0_26;               //取反
    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_1) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_1, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */
        PT1_17 = !PT1_17;               //取反
    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_2) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_2, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */
        PT0_28 = !PT0_28;
    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_3) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* 清除标志位 */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_3, kMRT_TimerInterruptFlag);
        
        /* 添加用户代码 */
        PT0_27 = !PT0_27;
        LED_ColorReverse(blue);    //蓝灯
    }
}





