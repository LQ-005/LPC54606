/*!
  * @file     EKF.c
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
#include "EKF.h"
#include "ComplementaryFilter.h"
#include "LowPassFilter.h"
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
  * @brief    卡尔曼滤波器初始化
  *
  * @param    param    ： 卡尔曼参数结构体
  * @param    dt       ： 两次卡尔曼之间的时间间隔  单位S
  * @param    Q        ： 卡尔曼参数   
  * @param    R        ： 卡尔曼参数 
  *
  * @return   
  *
  * @note     其实是模型误差与测量误差的大小，是模型预测值与测量值的加权。
  *           举例而言，R固定，Q越大，代表越信任侧量值，Q无穷代表只用测量值；
  *           反之，Q越小代表越信任模型预测值，Q为零则是只用模型预测。
  *           状态延时高，说明收敛速度慢。
  *           估计参数Q越大，收敛的越快。
  *           测量误差R越小，收敛的越快。
  *           调整这两个参数即可，从状态更新上说，测量误差越小，估计参数误差越大，说明我们越相信测量值，
  *           自然收敛的快。缺点就是会让系统变化过快，如果测量值更加不准，则精度会下降，系统不够稳定。
  *
  * @example  
  *
  * @date     2019/6/19 星期三
  */
void KalmanFilter_Init(ekf_param_t *param, float dt, float Q, float R)
{
    param->angle = 0;
    param->dt = dt;
    param->PP[0][0] = 1;
    param->PP[0][1] = 0;
    param->PP[1][0] = 0;
    param->PP[1][1] = 1;
    param->Q_angle = Q;
    param->Q_bias = 0;
    param->Q_gyro = 0.0001;
    param->R_angle = R;
}


/**
  * @brief    卡尔曼滤波
  *
  * @param    param    ： 卡尔曼参数结构体
  * @param    acc_angle： 加速度计 计算出的角度
  * @param    gyro：   ： 角速度
  *
  * @return   
  *
  * @note     仅供参考
  *
  * @example  
  *
  * @date     2019/6/19 星期三
  */
float KalmanFilter_apply(ekf_param_t *param, float acc_angle, float gyro)
{
    /* 陀螺仪的预测值 */
    float gyro_angle = param->angle + (gyro - param->Q_bias) * param->dt;

    /* 获取先验估计误差协方差的微分 */
    float Pdot[4];
    Pdot[0] = param->Q_angle - param->PP[0][1] - param->PP[1][0];     
	Pdot[1] = -param->PP[1][1];
	Pdot[2] = -param->PP[1][1];
	Pdot[3] = param->Q_gyro;
    
    /* 对先验估计误差协方差的微分进行积分 获得误差协方差 */
    param->PP[0][0] += Pdot[0] * param->dt;   
    param->PP[0][1] += Pdot[1] * param->dt;   
    param->PP[1][0] += Pdot[2] * param->dt;
    param->PP[1][1] += Pdot[3] * param->dt;
    
    /* 计算卡尔曼增益 */
    float E = param->R_angle + param->PP[0][0];
    
    float k_0 = param->PP[0][0] / E;
    float k_1 = param->PP[1][0] / E;
    
    
    /* 最优估计 */
    float angle_err = acc_angle - gyro_angle;
 
    param->angle = gyro_angle + k_0 * angle_err;
    param->Q_bias = param->Q_bias + k_1 * angle_err;
    
    /* 更新后验估计误差协方差 */
    float temp1 = param->PP[0][0];
    float temp2 = param->PP[1][0];

    param->PP[0][0] -= k_0 * temp1;
    param->PP[0][1] -= k_0 * temp2;
    param->PP[1][0] -= k_1 * temp1;
    param->PP[1][1] -= k_1 * temp2;
        
    return param->angle;
}


/* 定义一个卡尔曼滤波器 */
ekf_t ekf = 
{
    KalmanFilter_Init,
    KalmanFilter_apply,
};




/**
  * @brief    测试滤波
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
void Test_EKF(void)
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
	float raw_comp_angle, raw_ekf_angle;
	
	complementary_filter_param_t raw_filter_param;
    ekf_param_t raw_ekf_param;
	
	/* 参数需要自行调节 */
	complementary_filter.init(&raw_filter_param, 0.01, 0.005);
    ekf.init(&raw_ekf_param, 0.005, 0.1, 5);
    
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
            
            /* 用原始数据卡尔曼滤波 */
            raw_ekf_angle = ekf.get_angle(&raw_ekf_param, RawAngle, -ins.get_gyro().y * RadtoDeg);
			
            if(count++ % 2 == 0)
            {
                LED_ColorReverse(blue);
                ANO_DT_send_int16((short)RawAngle, (short)raw_comp_angle, (short)raw_ekf_angle, 0, 0, 0, 0, 0);
            }
        }
       
    }
}





