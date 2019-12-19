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

/**
  * @brief    PIN_INT2_IRQHandler中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     摄像头场中断例程使用该函数
  *
  * @example  
  *
  * @date     2019/6/4 星期二
  */
void PIN_INT7_IRQHandler(void)
{ 
    /* 用户代码 */
    camera_flag = false;

	/* 重新配置DMA描述符 */
    DMA_CameraReload(&PT1__BYTE3, (uint8_t *)Image_Data);
    
    PINT_PinInterruptClrStatus(PINT, kPINT_PinInt7);      //清除中断标志位

    __DSB();				                              //数据同步屏蔽指令 
} 



