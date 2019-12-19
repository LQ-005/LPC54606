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
  * @brief    flexcom0中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     测试串口使用
  *
  * @see      
  *
  * @date     2019/10/31 星期四
  */
void FLEXCOMM0_IRQHandler(void)
{
    uint8_t data;
    /* 接收中断. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(USART0))
    {
        data = UART_GetChar(USART0);
        printf("接收到字符： %c \n", data);
    } 
}


/*!
  * @brief    flexcom3中断服务函数
  *
  * @param    
  *
  * @return   
  *
  * @note     测试串口使用
  *
  * @see      
  *
  * @date     2019/10/31 星期四
  */
void FLEXCOMM3_IRQHandler(void)
{
    char txt[16];
    uint8_t data;
    /* 接收中断. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(USART3))
    {
        data = UART_GetChar(USART3);
        sprintf(txt, "接收到字符： %c \n", data);
        UART_PutStr(USART3, txt);
    } 
}



