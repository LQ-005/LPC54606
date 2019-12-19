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

/* */
static volatile uint8_t wdt_update_count;


/**
  * @brief    ���Ź��ж�
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @example  
  *
  * @date     2019/5/7 ���ڶ�
  */
void WDT_BOD_IRQHandler(void)
{
    uint32_t wdtStatus = WWDT_GetStatusFlags(WWDT);

    LED_ColorReverse(red);     //�����˸

    //��Ƭ��֮ǰ��λ
    if (wdtStatus & kWWDT_TimeoutFlag)
    {        
        WWDT_Disable(WWDT);//ֹͣ���Ź�
        WWDT_ClearStatusFlags(WWDT, kWWDT_TimeoutFlag);//״̬����        
        WWDT_Enable(WWDT);//�������Ź�
    }

    //���洦��
    if (wdtStatus & kWWDT_WarningFlag)
    {
        //����֮ǰû��ι��
        WWDT_ClearStatusFlags(WWDT, kWWDT_WarningFlag);
        if (wdt_update_count < 5)
        {
            //5�ξ�����������Ź���λ����
            WWDT_Refresh(WWDT);
            wdt_update_count++;
        }
    }
}