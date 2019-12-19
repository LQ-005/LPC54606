#ifndef __COMPLEMENTARYFILTER_H_H
#define __COMPLEMENTARYFILTER_H_H
#include "fsl_common.h"

typedef struct
{
    float k;      //�����˲�ϵ��
    float dt;     //���λ����˲�֮���ʱ���� ��λS
    float angle;  //�����˲�������ĽǶ�
}complementary_filter_param_t;


typedef struct
{
    void (* init)(complementary_filter_param_t *param, float k, float dt);
    float (* get_angle)(complementary_filter_param_t *param, float acc_angle, float gyro);
}complementary_filter_t;


extern complementary_filter_t  complementary_filter;
    
/**
  * @brief    
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/18 ���ڶ�
  */
void Test_ComplementaryFilter(void);


#endif
