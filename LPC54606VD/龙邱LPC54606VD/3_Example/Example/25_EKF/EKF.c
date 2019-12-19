/*!
  * @file     EKF.c
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

#define RadtoDeg    57.324841f				//���ȵ��Ƕ� (���� * 180/3.1415)
#define DegtoRad    0.0174533f				//�Ƕȵ����� (�Ƕ� * 3.1415/180)

/**
  * @brief    �������˲�����ʼ��
  *
  * @param    param    �� �����������ṹ��
  * @param    dt       �� ���ο�����֮���ʱ����  ��λS
  * @param    Q        �� ����������   
  * @param    R        �� ���������� 
  *
  * @return   
  *
  * @note     ��ʵ��ģ�������������Ĵ�С����ģ��Ԥ��ֵ�����ֵ�ļ�Ȩ��
  *           �������ԣ�R�̶���QԽ�󣬴���Խ���β���ֵ��Q�������ֻ�ò���ֵ��
  *           ��֮��QԽС����Խ����ģ��Ԥ��ֵ��QΪ������ֻ��ģ��Ԥ�⡣
  *           ״̬��ʱ�ߣ�˵�������ٶ�����
  *           ���Ʋ���QԽ��������Խ�졣
  *           �������RԽС��������Խ�졣
  *           �����������������ɣ���״̬������˵���������ԽС�����Ʋ������Խ��˵������Խ���Ų���ֵ��
  *           ��Ȼ�����Ŀ졣ȱ����ǻ���ϵͳ�仯���죬�������ֵ���Ӳ�׼���򾫶Ȼ��½���ϵͳ�����ȶ���
  *
  * @example  
  *
  * @date     2019/6/19 ������
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
  * @brief    �������˲�
  *
  * @param    param    �� �����������ṹ��
  * @param    acc_angle�� ���ٶȼ� ������ĽǶ�
  * @param    gyro��   �� ���ٶ�
  *
  * @return   
  *
  * @note     �����ο�
  *
  * @example  
  *
  * @date     2019/6/19 ������
  */
float KalmanFilter_apply(ekf_param_t *param, float acc_angle, float gyro)
{
    /* �����ǵ�Ԥ��ֵ */
    float gyro_angle = param->angle + (gyro - param->Q_bias) * param->dt;

    /* ��ȡ����������Э�����΢�� */
    float Pdot[4];
    Pdot[0] = param->Q_angle - param->PP[0][1] - param->PP[1][0];     
	Pdot[1] = -param->PP[1][1];
	Pdot[2] = -param->PP[1][1];
	Pdot[3] = param->Q_gyro;
    
    /* ������������Э�����΢�ֽ��л��� ������Э���� */
    param->PP[0][0] += Pdot[0] * param->dt;   
    param->PP[0][1] += Pdot[1] * param->dt;   
    param->PP[1][0] += Pdot[2] * param->dt;
    param->PP[1][1] += Pdot[3] * param->dt;
    
    /* ���㿨�������� */
    float E = param->R_angle + param->PP[0][0];
    
    float k_0 = param->PP[0][0] / E;
    float k_1 = param->PP[1][0] / E;
    
    
    /* ���Ź��� */
    float angle_err = acc_angle - gyro_angle;
 
    param->angle = gyro_angle + k_0 * angle_err;
    param->Q_bias = param->Q_bias + k_1 * angle_err;
    
    /* ���º���������Э���� */
    float temp1 = param->PP[0][0];
    float temp2 = param->PP[1][0];

    param->PP[0][0] -= k_0 * temp1;
    param->PP[0][1] -= k_0 * temp2;
    param->PP[1][0] -= k_1 * temp1;
    param->PP[1][1] -= k_1 * temp2;
        
    return param->angle;
}


/* ����һ���������˲��� */
ekf_t ekf = 
{
    KalmanFilter_Init,
    KalmanFilter_apply,
};




/**
  * @brief    �����˲�
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
#error "����config.h ��ȷ�������ǽ��߷�ʽ����ʱֻ֧����Ӳ��IIC"
#endif
    
    systime.init();
    
	float RawAngle = 0;
	float raw_comp_angle, raw_ekf_angle;
	
	complementary_filter_param_t raw_filter_param;
    ekf_param_t raw_ekf_param;
	
	/* ������Ҫ���е��� */
	complementary_filter.init(&raw_filter_param, 0.01, 0.005);
    ekf.init(&raw_ekf_param, 0.005, 0.1, 5);
    
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
            
            /* ��ԭʼ���ݿ������˲� */
            raw_ekf_angle = ekf.get_angle(&raw_ekf_param, RawAngle, -ins.get_gyro().y * RadtoDeg);
			
            if(count++ % 2 == 0)
            {
                LED_ColorReverse(blue);
                ANO_DT_send_int16((short)RawAngle, (short)raw_comp_angle, (short)raw_ekf_angle, 0, 0, 0, 0, 0);
            }
        }
       
    }
}





