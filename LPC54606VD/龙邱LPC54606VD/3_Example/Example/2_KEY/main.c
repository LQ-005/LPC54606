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

    PRINTF("\r\n �����������ܿƼ� \r\n");
    printf("\r\n �����������ܿƼ� \r\n");
    
    /*�����ж����ȼ���  0: 0����ռ���ȼ� 8�������ȼ� 
     *                  1: 2����ռ���ȼ� 4�������ȼ� 
	 *		            2: 4����ռ���ȼ� 2�������ȼ� 
     *                  3: 8����ռ���ȼ� 0�������ȼ� 
     */
	/* �������ȼ����� 3: 8����ռ���ȼ� 0-7  0���ȼ���� */
    NVIC_SetPriorityGrouping(NVIC_Group3);
    
    /*!
      * @brief   ����ĸ���ϵİ���  ����ĸ���ϵİ��� ���İ��ϵ�LED����˸
      *
      * @note    ����ʹ�õĹܽ���LQ_KEY.h�� �궨��ѡ��
      */ 
    Test_KEY();
    
    while (1)
    {
        
    }
}
