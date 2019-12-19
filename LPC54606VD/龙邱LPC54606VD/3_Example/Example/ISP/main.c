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
  * @note     LPC54606 ��32KB��I&D SRAM  �����ͨ��SRAM�ٶȿ�ܶ� 
  * @note     �ڷ�ɢ�����ļ��� Ĭ�Ͻ�32K��I&D SRAM�ֳ�16K��ջ����16K��RamFunction
  * @note     ����ʹ��RAMFUNCTION_SECTION_CODE()���ؼ����ִ���ŵ�I&D SRAM������� ʵ���ٶȻ��һ��
  * @note     ����ֻ����һ��ʾ���õ� 
  *
  * @example  delayms(100);
  *
  * @date     2019/4/22 ����һ
*/
RAMFUNCTION_SECTION_CODE(void delayms(uint16_t ms))
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

#include "fsl_otp.h"
/*!
 * @brief �ر�ISP ���� �ر�֮�󲻿���  �ر�֮�󲻿���ͨ������ SPI �ȷ�ʽ���س���ֻ��������������
 */
int main(void)
{  
    /* ʹ��OTP  */
    CLOCK_EnableClock(kCLOCK_Otp);
    
    /* ��λ */
    RESET_PeripheralReset(kOTP_RST_SHIFT_RSTn);
    
    /* ��ʼ��ʱ�� 12M */
    BOARD_BootClockFRO12M();
    
    /* ��ʼ�� ����0 */
    BOARD_InitDebugConsole();
    
    /* Get version of driver in ROM */
    uint32_t version = OTP_GetDriverVersion();
    PRINTF("OTP ROM API driver version: 0x%X\r\n\r\n", version);
    
    PRINTF("�ر�ISP ���� �ر�֮�󲻿���  �ر�֮�󲻿���ͨ������ SPI �ȷ�ʽ���س���ֻ��������������\r\n\r\n");
    
    /* ��ʼ��OTP */
    OTP_Init();
    
    /* ʹ�ܶ� */
    OTP_EnableBankReadLock(3, kOTP_Word0, kOTP_Word1, kOTP_LockDontLock);
    
    PRINTF("OTP BANK3 Word0: 0x%X\r\n\r\n", OTPC->ECRP);
    /* �Ƿ��Ѿ��ر� ISP */
    if((OTPC->ECRP & OTPC_ECRP_CRP_ISP_DISABLE_PIN_MASK))
    {
        PRINTF("ISP �Ѿ��ر� \n\r");
    }
    else /* �벻Ҫ�޸� ����Ĳ������ǲ������ ��Ҫ������Ӧ���� ���Բο� �ֲ�45�� OTP�½� */
    {
        OTP_EnableBankWriteMask(kOTP_Bank3);
        OTP_EnableBankWriteLock(3, kOTP_Word0, kOTP_Word1, kOTP_LockDontLock);
        OTP_ProgramRegister(3,0,0x40);
        if((OTPC->ECRP & OTPC_ECRP_CRP_ISP_DISABLE_PIN_MASK))
        {
            PRINTF("ISP �Ѿ��ر� \n\r");
        }
        else
        {
            PRINTF("ISP ����ʧ�� \n\r");
        }
        OTP_DisableBankWriteMask(kOTP_Bank3);
    }
    /* */
    while(1)
    {

    }
}
