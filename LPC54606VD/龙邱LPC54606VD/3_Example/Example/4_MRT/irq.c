/*!
  * @file     irq.c
  *
  * @brief    �жϷ�����
  *
  * @company  �����������ܿƼ�
  *
  * @author   LQ-005
  *
  * @note     �жϷ����� ��֪�����ֿ���ȥstartup_LPC54606.s������
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

/*!
  * @brief    HardFault�жϷ�����
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @see      
  *
  * @date     2019/11/4 ����һ
  */
void HardFault_Handler(void)
{
    PRINTF("�������һ�㶼������Խ����߲�����δ��ʼ��������\r\n");
    PRINTF("�ڳ���ʼ�׶Σ����Թر�ȫ���ж� DisableInterrupts  ���������趼��ʼ����ɺ��ٿ���ȫ���ж� EnableInterrupts\r\n");
    PRINTF("��������жϷ������в���δ��ʼ������\r\n");
    PRINTF("�����ʼ����MRT��ʱ�� ��MRT�ж���ʹ�ô���1��ӡ������MRT�ȳ�ʼ���������ж�ʱ����1��δ��ʼ�����͵����˲���δ��ʼ���Ĵ���1���ͻ��������\r\n");
    while(1);
}


/*!
  * @brief    MRT�жϷ�����
  *
  * @param    ��
  *
  * @return   
  *
  * @note     Test_MRT()����ʹ��
  *
  * @see      
  *
  * @date     2019/10/31 ������
  */
void MRT0_IRQHandler(void)
{
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_0) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* �����־λ */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_0, kMRT_TimerInterruptFlag);
        
        /* ����û����� */
        PT0_26 = !PT0_26;               //ȡ��
    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_1) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* �����־λ */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_1, kMRT_TimerInterruptFlag);
        
        /* ����û����� */
        PT1_17 = !PT1_17;               //ȡ��
    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_2) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* �����־λ */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_2, kMRT_TimerInterruptFlag);
        
        /* ����û����� */
        PT0_28 = !PT0_28;
    }
    
    if(MRT_GetStatusFlags(MRT0, kMRT_Channel_3) & MRT_CHANNEL_STAT_INTFLAG_MASK)
    {
        /* �����־λ */
        MRT_ClearStatusFlags(MRT0, kMRT_Channel_3, kMRT_TimerInterruptFlag);
        
        /* ����û����� */
        PT0_27 = !PT0_27;
        LED_ColorReverse(blue);    //����
    }
}





