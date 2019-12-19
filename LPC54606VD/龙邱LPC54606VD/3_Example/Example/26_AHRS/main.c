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
      * @brief   测试姿态解算 MPU6050、ICM20602、九轴IIC接线 config.h选择IIC方式  
      *          3V3  ---  3.3V
      *          GND  ---  GND
      *         
      *          如果config.h选择模拟IIC接口
      *          SCL  ---  P0_2
      *          SDA  ---  P0_3

      *          如果config.h选择硬件IIC接口
      *          SCL  ---  P0_14
      *          SDA  ---  P0_13 
      *          本例程默认使用硬件IIC 读取原始数据  以5ms的周期进行姿态解算 并通过串口+DMA发送到匿名上位机显示波形
      *          注意 陀螺仪要正面朝上  否则数据不准，需要自行修改程序
      * @note    通过串口0 发送给匿名上位机显示波形   如果不会使用匿名上位机 可以参考例程 23_ANO_DT
      */ 
#include "AHRS.h"
    Test_ahrs();
    
    while(1)
    {

    }
}
