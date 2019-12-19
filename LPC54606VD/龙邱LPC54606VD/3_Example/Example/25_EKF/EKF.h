#ifndef __EKF_H
#define __EKF_H

#include "fsl_common.h"


typedef struct
{
    float angle;         //卡尔曼输出角度
    float dt;            //两次卡尔曼滤波之间的时间间隔 单位s
    float Q_angle;       //陀螺仪角度噪声的协方差
    float Q_gyro;        //陀螺仪角速度漂移噪声的协方差
    float Q_bias;        //陀螺仪漂移
    float R_angle;       //加速度计角度噪声协方差
    float PP[2][2];      //协方差矩阵
       
}ekf_param_t;


typedef struct
{
    void (* init)(ekf_param_t *param, float dt, float Q, float R);
    float (* get_angle)(ekf_param_t *param, float acc_angle, float gyro);
}ekf_t;



extern ekf_t ekf;

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
void Test_EKF(void);


#endif

