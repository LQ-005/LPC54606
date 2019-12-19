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
      * @brief   测试编码器    LPC54606 只能使用带方向编码器
      *          编码器1的方向接P0_1   脉冲接P1_11
      *          编码器2的方向接P0_24  脉冲接P1_21
      *          编码器3的方向接P0_4   脉冲接P1_16
      *          编码器4的方向接P0_16  脉冲接P1_8
      *          市面上的编码器， 都会说是多少线的， 比如 512 线编码器意思就是编码器旋转一圈产生 512
      *          个脉冲， 通过 ENC 可以读取到数值 512。 这个值和小车的速度是线性关系的， 一般情况下可以
      *          不去换算具体速度直接使用即可。 如果想要换算成具体速度， 则需要知道小车轮子周长 X0、
      *          小车齿轮 X1 和编码器齿轮的数目 X2、 编码器的线数 X3、 ENC 读到数据 X4 和 ENC 读取时间间
      *          隔 X5。 速度 V = ((X4/X3) * X2 / X1) * X0 / X5。
      * @note    屏幕上显示编码器读数，并通过串口0打印  config.h中选择屏幕
      */ 
    TEST_ENC();
    
    while(1)
    {
    
    }
}
