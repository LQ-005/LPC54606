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
  * @note     LPC54606 有32KB的I&D SRAM  相比普通的SRAM速度快很多 
  * @note     在分散加载文件里 默认将32K的I&D SRAM分成16K的栈区和16K的RamFunction
  * @note     可以使用RAMFUNCTION_SECTION_CODE()将关键部分代码放到I&D SRAM里面加速 实测速度会快一倍
  * @note     这里只是做一个示例用的 
  *
  * @example  delayms(100);
  *
  * @date     2019/4/22 星期一
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
 * @brief 关闭ISP 慎用 关闭之后不可逆  关闭之后不可以通过串口 SPI 等方式下载程序，只能用下载器下载
 */
int main(void)
{  
    /* 使能OTP  */
    CLOCK_EnableClock(kCLOCK_Otp);
    
    /* 复位 */
    RESET_PeripheralReset(kOTP_RST_SHIFT_RSTn);
    
    /* 初始化时钟 12M */
    BOARD_BootClockFRO12M();
    
    /* 初始化 串口0 */
    BOARD_InitDebugConsole();
    
    /* Get version of driver in ROM */
    uint32_t version = OTP_GetDriverVersion();
    PRINTF("OTP ROM API driver version: 0x%X\r\n\r\n", version);
    
    PRINTF("关闭ISP 慎用 关闭之后不可逆  关闭之后不可以通过串口 SPI 等方式下载程序，只能用下载器下载\r\n\r\n");
    
    /* 初始化OTP */
    OTP_Init();
    
    /* 使能读 */
    OTP_EnableBankReadLock(3, kOTP_Word0, kOTP_Word1, kOTP_LockDontLock);
    
    PRINTF("OTP BANK3 Word0: 0x%X\r\n\r\n", OTPC->ECRP);
    /* 是否已经关闭 ISP */
    if((OTPC->ECRP & OTPC_ECRP_CRP_ISP_DISABLE_PIN_MASK))
    {
        PRINTF("ISP 已经关闭 \n\r");
    }
    else /* 请不要修改 这里的操作都是不可逆的 想要操作对应功能 可以参考 手册45章 OTP章节 */
    {
        OTP_EnableBankWriteMask(kOTP_Bank3);
        OTP_EnableBankWriteLock(3, kOTP_Word0, kOTP_Word1, kOTP_LockDontLock);
        OTP_ProgramRegister(3,0,0x40);
        if((OTPC->ECRP & OTPC_ECRP_CRP_ISP_DISABLE_PIN_MASK))
        {
            PRINTF("ISP 已经关闭 \n\r");
        }
        else
        {
            PRINTF("ISP 操作失败 \n\r");
        }
        OTP_DisableBankWriteMask(kOTP_Bank3);
    }
    /* */
    while(1)
    {

    }
}
