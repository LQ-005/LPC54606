/*!
  * @file     ComplementaryFilter.c
  *
  * @brief    
  *
  * @company  北京龙邱智能科技
  *
  * @author   LQ-005
  *
  * @note     Tab键 4个空格
  *
  * @version  V1.0  2019/11/29  优化了模拟IIC支持
  *
  * @Software IAR8.32 or MDK5.25
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2019/11/11 星期一
  */ 
#include "ComplementaryFilter.h"
#include "Inertial_Sensor.h"
#include "LQ_SYSTICK.h"
#include "AP_Math.h"
#include "math.h"
#include "config.h"
#include "fsl_debug_console.h"
#include "LQ_SPI.h"
#include "ANO_DT.h"
#include "LQ_IIC.h"
#include "LQ_LED.h"
#include "LQ_UART_DMA.h"
#include "LQ_SOFTIIC.h"
#define RadtoDeg    57.324841f				//弧度到角度 (弧度 * 180/3.1415)
#define DegtoRad    0.0174533f				//角度到弧度 (角度 * 3.1415/180)

/**
  * @brief    滤波器初始化
  *
  * @param    param  : 存放滤波器参数
  * @param    k      : 相信加速度计的程度  0 - 1之间 请自行调试
  * @param    dt     : 两次计算角度的时间间隔  单位s
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/18 星期二
  */
void ComplementaryFilter_Init(complementary_filter_param_t *param, float k, float dt)
{
    param->k = k;
    param->dt = dt;
    param->angle = 0.0;
}


/**
  * @brief    互补滤波
  *
  * @param    param  :  存放滤波器参数
  * @param    angle  ： 当前通过加速度计算出的角度
  * @param    gyro   ： 陀螺仪角速度计积分获得的角度
  *
  * @return   
  *
  * @note     加速度计和重力相关，只要重力不变，解算出的角度就不会有太大误差，但是会在一定范围内波动（加速度值容易乱跳，有高频噪声）
  *           并且加速度计对角度变化反应灵敏，因此更容易受到噪声干扰。
  *           角速度计 可以通过积分算角度，但是如果每次积分都有一点点误差，当时间长了，这个误差就十分恐怖了
  *           互补滤波的核心思想就是 利用加速度计计算出的角度修正 角速度计积分误差
  *
  * @example  
  *
  * @date     2019/6/18 星期二
  */
float ComplementaryFilter_GetAngle(complementary_filter_param_t *param, float acc_angle, float gyro)
{
    param->angle = param->k * acc_angle + (1 - param->k) * (param->angle + gyro * param->dt);
    return param->angle;
}

/* 定义一个互补滤波器 */
complementary_filter_t  complementary_filter = 
{
    ComplementaryFilter_Init,
    ComplementaryFilter_GetAngle,
};


/**
  * @brief    测试互补滤波
  *
  * @param    
  *
  * @return   
  *
  * @note     参数需要自行调节
  *
  * @example  
  *
  * @date     2019/6/18 星期二
  */
void Test_ComplementaryFilter(void)
{  
    uint16_t count = 0;
    uint32_t current_time;   
	uint32_t last_time;
    
#if (IMU_USE_SELECT == USE_HAL_IIC)
    IIC_InitConfig(I2C1, 400000);
#elif (IMU_USE_SELECT == USE_SOFT_IIC) 
    IIC_Init(); 
#else
#error "请在config.h 中确定陀螺仪接线方式，暂时只支持软、硬件IIC"
#endif
    
    systime.init();
    
	float RawAngle = 0;
	float raw_comp_angle;
	
	complementary_filter_param_t raw_filter_param;
    /* 参数需要自行调节 */
    complementary_filter.init(&raw_filter_param, 0.01, 0.005);
    LED_Init();
#if ANO_DT_USE_DMA
	UART_DMAInitConfig(USART0, 115200);
#else
	UART_InitConfig(USART0, 115200);   
    //优先级配置 抢占优先级5  子优先级0   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(FLEXCOMM0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
    EnableIRQ(FLEXCOMM0_IRQn);	                            //使能FLEXCOMM0_IRQn中断
#endif
    if(ins.init())
    {
        PRINTF("IMU init fail ! \n");
        while(1);
    }
    while(1)
    {
        current_time = systime.get_time_us() - last_time;   //当前时间减去上一个周期的开始时间
        if(current_time >= 5000)                    // 一个周期5ms
        {
            last_time = systime.get_time_us();      //记住开始时间
            ins.update();                           //更新数据
            
            /* 用原始加速度的值 计算角度 */
			RawAngle = ( atan2(ins.get_acc().x, ins.get_acc().z) * 180 / M_PI); 		   
            	        
			/* 用原始数据互补滤波计算角度 注意方向问题 */
            raw_comp_angle = complementary_filter.get_angle(&raw_filter_param, RawAngle, -ins.get_gyro().y * RadtoDeg);
            	
            if(count++ % 2 == 0)
            {
                LED_ColorReverse(blue);
                ANO_DT_send_int16((short)RawAngle, (short)raw_comp_angle, 0, 0, 0, 0, 0, 0);
            }
        }
       
    }
}

