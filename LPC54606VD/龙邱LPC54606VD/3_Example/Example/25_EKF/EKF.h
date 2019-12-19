#ifndef __EKF_H
#define __EKF_H

#include "fsl_common.h"


typedef struct
{
    float angle;         //����������Ƕ�
    float dt;            //���ο������˲�֮���ʱ���� ��λs
    float Q_angle;       //�����ǽǶ�������Э����
    float Q_gyro;        //�����ǽ��ٶ�Ư��������Э����
    float Q_bias;        //������Ư��
    float R_angle;       //���ٶȼƽǶ�����Э����
    float PP[2][2];      //Э�������
       
}ekf_param_t;


typedef struct
{
    void (* init)(ekf_param_t *param, float dt, float Q, float R);
    float (* get_angle)(ekf_param_t *param, float acc_angle, float gyro);
}ekf_t;



extern ekf_t ekf;

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
void Test_EKF(void);


#endif

