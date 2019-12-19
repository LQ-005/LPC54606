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
      * @brief   ���Ա�����    LPC54606 ֻ��ʹ�ô����������
      *          ������1�ķ����P0_1   �����P1_11
      *          ������2�ķ����P0_24  �����P1_21
      *          ������3�ķ����P0_4   �����P1_16
      *          ������4�ķ����P0_16  �����P1_8
      *          �����ϵı������� ����˵�Ƕ����ߵģ� ���� 512 �߱�������˼���Ǳ�������תһȦ���� 512
      *          �����壬 ͨ�� ENC ���Զ�ȡ����ֵ 512�� ���ֵ��С�����ٶ������Թ�ϵ�ģ� һ������¿���
      *          ��ȥ��������ٶ�ֱ��ʹ�ü��ɡ� �����Ҫ����ɾ����ٶȣ� ����Ҫ֪��С�������ܳ� X0��
      *          С������ X1 �ͱ��������ֵ���Ŀ X2�� ������������ X3�� ENC �������� X4 �� ENC ��ȡʱ���
      *          �� X5�� �ٶ� V = ((X4/X3) * X2 / X1) * X0 / X5��
      * @note    ��Ļ����ʾ��������������ͨ������0��ӡ  config.h��ѡ����Ļ
      */ 
    TEST_ENC();
    
    while(1)
    {
    
    }
}
