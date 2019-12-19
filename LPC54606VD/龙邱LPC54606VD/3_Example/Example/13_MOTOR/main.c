/*!
  * @file     main.c
  *
  * @brief    
  *
  * @company  �����������ܿƼ�
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
  * @date     2019/10/19 ������
  */ 
#include "include.h"

/* �ж����ȼ��� */
#define NVIC_Group0   0x07
#define NVIC_Group1   0x06
#define NVIC_Group2   0x05
#define NVIC_Group3   0x04


/**
  * @brief    ����ȷ��ʱ
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @example  delayms(100);
  *
  * @date     2019/4/22 ����һ
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
    /* ��ʼ������ʱ�� 180M */
    BOARD_BootClockPLL180M();
    
    /* ��ʼ������0 ����ʹ�� PRINTF���� */
    BOARD_InitDebugConsole();

    
    /*�����ж����ȼ���  0: 0����ռ���ȼ� 8�������ȼ� 
     *                  1: 2����ռ���ȼ� 4�������ȼ� 
	 *		            2: 4����ռ���ȼ� 2�������ȼ� 
     *                  3: 8����ռ���ȼ� 0�������ȼ� 
     */
	/* �������ȼ����� 3: 8����ռ���ȼ� 0-7  0���ȼ���� */
    NVIC_SetPriorityGrouping(NVIC_Group3);
    
    /*!
      * @brief   ���Ե�� ������ʾ�����۲첨��  
      *          ע��һ���������·PWM���ƣ�һ·PWMռ�ձȸ�0��һ·��PWM�źţ�PWM�ź�ռ�ձ�Խ�󣬵�������������ѹԽ�󣬵��Ť��Խ��
      *          �����ת������·PWM�źŶԵ����ɡ���������ϵ��������PWM֮�� һ�����һ����ԴV���룬����Ǹ������ϵĹ����ģ���3.3V����
      *          1�����1�� ʹ�� P0_26  P1_17 
      *          2�����2�� ʹ�� P0_28  P0_27  
      *          3�����3�� ʹ�� P0_19  P0_22  
      *          4�����4�� ʹ�� P0_18  P0_17  
      *
      * @note    ���� K1���� ���ֹͣ  K0��K2���£�����Ӽ���
      */ 
    TEST_MOTOR();
    
    while(1)
    {
    
    }
}
