/*!
  * @file     LQ_WDOG.c
  *
  * @brief    看门狗驱动文件
  *
  * @company  北京龙邱智能科技
  *
  * @author   LQ-005
  *
  * @note     无
  *
  * @version  V1.1  2019/12/06 优化注释 Doxygen
                    2020/01/06 优化WDOG_InitConfig函数参数
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2019/10/18 星期五
  */ 
#include "fsl_power.h"
#include "fsl_clock.h"
#include "fsl_wwdt.h"
#include "LQ_WDOG.h"


/*!
  * @brief    看门狗初始化
  *
  * @param    min_ms    ：窗口下限时间 单位 ms
  * @param    max_ms    ：窗口上限时间 单位 ms
  *
  * @return   无
  *
  * @note     wwdt是一个24位的定时器 默认时钟125KHz  max_ms最大 134217  
  *
  * @see      WDOG_InitConfig(2000, 5000); //设置看门狗 在2-5S内喂狗
  * @see      优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级高的可打断抢占优先级低的中断
  * @see      NVIC_SetPriority(WDT_BOD_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
  * @see      EnableIRQ(WDT_BOD_IRQn);  //使能中断
  *
  * @date     2019/10/19 星期六
  */
void WDOG_InitConfig(uint32_t min_ms, uint32_t max_ms)
{
    /* 检查参数 wwdt 是24位定时器 */
	
	
    POWER_DisablePD(kPDRUNCFG_PD_WDT_OSC);
    
    wwdt_config_t config;
    
    WWDT_GetDefaultConfig(&config);
    
    /* 设置窗口上限 */
    config.timeoutValue = CLOCK_GetFreq(kCLOCK_WdtOsc) / 4000 * max_ms;
    
    /* 设置警告时间 */
    config.warningValue = 512;
    
    /* 设置窗口下限 */
    config.windowValue  = CLOCK_GetFreq(kCLOCK_WdtOsc) / 4000 * min_ms;
    
    /* 使能超时复位  */
    config.enableWatchdogReset = true;
    
    /* 设置看门狗时钟. */
    config.clockFreq_Hz = CLOCK_GetFreq(kCLOCK_WdtOsc);
    
    /* 初始化 */
    WWDT_Init(WWDT, &config);
    
    /* 喂狗 开始 */
    WWDT_Refresh(WWDT);
    
}
