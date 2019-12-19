/*!
  * @file     ComplementaryFilter.c
  *
  * @brief    
  *
  * @company  �����������ܿƼ�
  *
  * @author   LQ-005
  *
  * @note     Tab�� 4���ո�
  *
  * @version  V1.0  2019/11/29  �Ż���ģ��IIC֧��
  *
  * @Software IAR8.32 or MDK5.25
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2019/11/11 ����һ
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
#define RadtoDeg    57.324841f				//���ȵ��Ƕ� (���� * 180/3.1415)
#define DegtoRad    0.0174533f				//�Ƕȵ����� (�Ƕ� * 3.1415/180)

/**
  * @brief    �˲�����ʼ��
  *
  * @param    param  : ����˲�������
  * @param    k      : ���ż��ٶȼƵĳ̶�  0 - 1֮�� �����е���
  * @param    dt     : ���μ���Ƕȵ�ʱ����  ��λs
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/18 ���ڶ�
  */
void ComplementaryFilter_Init(complementary_filter_param_t *param, float k, float dt)
{
    param->k = k;
    param->dt = dt;
    param->angle = 0.0;
}


/**
  * @brief    �����˲�
  *
  * @param    param  :  ����˲�������
  * @param    angle  �� ��ǰͨ�����ٶȼ�����ĽǶ�
  * @param    gyro   �� �����ǽ��ٶȼƻ��ֻ�õĽǶ�
  *
  * @return   
  *
  * @note     ���ٶȼƺ�������أ�ֻҪ�������䣬������ĽǶȾͲ�����̫�������ǻ���һ����Χ�ڲ��������ٶ�ֵ�����������и�Ƶ������
  *           ���Ҽ��ٶȼƶԽǶȱ仯��Ӧ��������˸������ܵ��������š�
  *           ���ٶȼ� ����ͨ��������Ƕȣ��������ÿ�λ��ֶ���һ�������ʱ�䳤�ˣ��������ʮ�ֲֿ���
  *           �����˲��ĺ���˼����� ���ü��ٶȼƼ�����ĽǶ����� ���ٶȼƻ������
  *
  * @example  
  *
  * @date     2019/6/18 ���ڶ�
  */
float ComplementaryFilter_GetAngle(complementary_filter_param_t *param, float acc_angle, float gyro)
{
    param->angle = param->k * acc_angle + (1 - param->k) * (param->angle + gyro * param->dt);
    return param->angle;
}

/* ����һ�������˲��� */
complementary_filter_t  complementary_filter = 
{
    ComplementaryFilter_Init,
    ComplementaryFilter_GetAngle,
};


/**
  * @brief    ���Ի����˲�
  *
  * @param    
  *
  * @return   
  *
  * @note     ������Ҫ���е���
  *
  * @example  
  *
  * @date     2019/6/18 ���ڶ�
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
#error "����config.h ��ȷ�������ǽ��߷�ʽ����ʱֻ֧����Ӳ��IIC"
#endif
    
    systime.init();
    
	float RawAngle = 0;
	float raw_comp_angle;
	
	complementary_filter_param_t raw_filter_param;
    /* ������Ҫ���е��� */
    complementary_filter.init(&raw_filter_param, 0.01, 0.005);
    LED_Init();
#if ANO_DT_USE_DMA
	UART_DMAInitConfig(USART0, 115200);
#else
	UART_InitConfig(USART0, 115200);   
    //���ȼ����� ��ռ���ȼ�5  �����ȼ�0   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж�
    NVIC_SetPriority(FLEXCOMM0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5,0));
    EnableIRQ(FLEXCOMM0_IRQn);	                            //ʹ��FLEXCOMM0_IRQn�ж�
#endif
    if(ins.init())
    {
        PRINTF("IMU init fail ! \n");
        while(1);
    }
    while(1)
    {
        current_time = systime.get_time_us() - last_time;   //��ǰʱ���ȥ��һ�����ڵĿ�ʼʱ��
        if(current_time >= 5000)                    // һ������5ms
        {
            last_time = systime.get_time_us();      //��ס��ʼʱ��
            ins.update();                           //��������
            
            /* ��ԭʼ���ٶȵ�ֵ ����Ƕ� */
			RawAngle = ( atan2(ins.get_acc().x, ins.get_acc().z) * 180 / M_PI); 		   
            	        
			/* ��ԭʼ���ݻ����˲�����Ƕ� ע�ⷽ������ */
            raw_comp_angle = complementary_filter.get_angle(&raw_filter_param, RawAngle, -ins.get_gyro().y * RadtoDeg);
            	
            if(count++ % 2 == 0)
            {
                LED_ColorReverse(blue);
                ANO_DT_send_int16((short)RawAngle, (short)raw_comp_angle, 0, 0, 0, 0, 0, 0);
            }
        }
       
    }
}

