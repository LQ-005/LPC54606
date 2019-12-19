/*!
  * @file     AHRS.c
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
#include "AHRS.h"
#include "Inertial_Sensor.h"
#include "LQ_SYSTICK.h"
#include "AP_Math.h"
#include "math.h"
#include "include.h"
#include "fsl_debug_console.h"
#include "LQ_SPI.h"
#include "ANO_DT.h"
#include "LQ_IIC.h"
#include "LQ_LED.h"
#include "LQ_UART_DMA.h"
#include "LQ_SOFTIIC.h"
////////////////////////////////////////////////////////////////////////////////
// Orientation
////////////////////////////////////////////////////////////////////////////////
// Convienience accessors for commonly used trig functions. These values are generated
// by the DCM through a few simple equations. They are used throughout the code where cos and sin
// would normally be used.
// The cos values are defaulted to 1 to get a decent initial value for a level state
static Q4_t Q4 = {1, 0, 0, 0};  //四元数
const float ahrs_kp = 1.08f; //PI控制器，修正机体坐标系
const float ahrs_ki = 0.05f;
static vector3f_t integral;  //机体坐标误差积分
static vector3f_t ahrs_angle;  //机体坐标误差积分


/****函数  AHRS_quat_update
	*作用  更新四元数
	*参数
	*返回值
	***/
void AHRS_quat_update(vector3f_t gyro, vector3f_t acc, float interval)
{
	float q0 = Q4.q0;
	float q1 = Q4.q1;
	float q2 = Q4.q2;
	float q3 = Q4.q3;
/***********  模长  ************/	
	float norm = invSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
/***********  加计测出的机体坐标系   **********/
	float ax = acc.x * norm;
	float ay = acc.y * norm;
	float az = acc.z * norm;
/***********  四元数解算出的机体坐标系  ***************/
	float half_vx = q1*q3 - q0*q2;
	float half_vy = q2*q3 + q0*q1;
	float half_vz = -0.5f + q0*q0 + q3*q3;
/***********  叉积求加计机体坐标和上次四元数解算机体坐标差  ************/	
	float half_ex = ay*half_vz - az*half_vy;
	float half_ey = az*half_vx - ax*half_vz;
	float half_ez = ax*half_vy - ay*half_vx;
/***********  使用PI控制器 修正机体坐标 *************/	
	integral.x += half_ex * ahrs_ki * interval;
	integral.y += half_ey * ahrs_ki * interval;
	integral.z += half_ez * ahrs_ki * interval;
	
	float gx = (gyro.x + ahrs_kp * half_ex + integral.x) * 0.5f * interval;
	float gy = (gyro.y + ahrs_kp * half_ey + integral.y) * 0.5f * interval;
	float gz = (gyro.z + ahrs_kp * half_ez + integral.z) * 0.5f * interval;
	
/**********  更新四元数  ********/
	Q4.q0 += (-q1 * gx - q2 * gy - q3 * gz); 
	Q4.q1 += ( q0 * gx + q2 * gz - q3 * gy); 
	Q4.q2 += ( q0 * gy - q1 * gz + q3 * gx); 
	Q4.q3 += ( q0 * gz + q1 * gy - q2 * gx); 
  //单位化四元数 	
	norm = invSqrt(Q4.q0 * Q4.q0 + Q4.q1 * Q4.q1 + Q4.q2 * Q4.q2 + Q4.q3 * Q4.q3);
	
	Q4.q0 *= norm;
	Q4.q1 *= norm;
	Q4.q2 *= norm;
	Q4.q3 *= norm;
}



/****函数  AHRS_quat_to_angle
	*作用  更新姿态角
	*参数
	*返回值
	***/
void AHRS_quat_to_angle(void)
{
	float conv_x = 2.0f * (Q4.q0 * Q4.q2 - Q4.q1 * Q4.q3);  
	float conv_y = 2.0f * (Q4.q0 * Q4.q1 + Q4.q2 * Q4.q3);
	float conv_z = Q4.q0 * Q4.q0 - Q4.q1 * Q4.q1 - Q4.q2 * Q4.q2 + Q4.q3 * Q4.q3;
/*******  姿态解算  ********/
	ahrs_angle.x = fast_atan(conv_y * invSqrt(conv_x * conv_x + conv_z * conv_z)) * 57.2958f;
	ahrs_angle.y = asin(2 * (Q4.q0 * Q4.q2 - Q4.q3 * Q4.q1)) * 57.2958f;
	ahrs_angle.z = atan2(2 * (Q4.q0 * Q4.q3 + Q4.q1 * Q4.q2), 1 - 2 * (Q4.q2 * Q4.q2 + Q4.q3 * Q4.q3)) * 57.2958f;   
    
    ahrs_angle.x = constrain_float(ahrs_angle.x, -90, 90);
    ahrs_angle.y = constrain_float(ahrs_angle.y, -90, 90);
    
/*******  对yaw 进行修正 防止yaw漂移 ********/ 
    static float yaw_vlaue = 0;  
	static float yaw_last = 0;  
	
	/*  漂移导致yaw变化  变化率比较小 缺点会降低YAW灵敏度*/
    if(fabs(ahrs_angle.z - yaw_last) > 0.2f)
	{
		yaw_vlaue += (ahrs_angle.z - yaw_last);
	}
	yaw_last = ahrs_angle.z;
	ahrs_angle.z = yaw_vlaue;	
	
}


/*姿态更新*/
void ahrs_update()
{
	// 读取惯性传感器数据
	ins.update();
	
	// if the update call took more than 0.2 seconds then discard it,
    // otherwise we may move too far. This happens when arming motors 
    // in ArduCopter
	static uint64_t last_time = 0;
	float dt = (systime.get_time_us() - last_time)/1000000.0f;
	if (dt > 0.006f)                                              //姿态解算周期最大6ms，大于的自行修改
	{
		last_time = systime.get_time_us();
        return;
    }
	last_time = systime.get_time_us();

    // quat update
    AHRS_quat_update(gyro_vector, acc_vector, dt);


    // quat to angle
    AHRS_quat_to_angle();

}

AHRS_t AHRS =
{
	ahrs_update,

};

 //测试姿态解算   APM上移植过来的，智能车一般不需要解算全部的姿态
void Test_ahrs(void)   
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
            last_time = systime.get_time_us();     //记住开始时间
            AHRS.update();
            LED_ColorReverse(blue);
            if(count++ % 2 == 0)
            {
                ANO_DT_send_int16((short)ahrs_angle.x, (short)ahrs_angle.y, (short)ahrs_angle.z, 0, 0, 0, 0, 0);
            }
        }
       
    }
}

