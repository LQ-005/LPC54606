#ifndef __COMPLEMENTARYFILTER_H_H
#define __COMPLEMENTARYFILTER_H_H
#include "fsl_common.h"

typedef struct
{
    float k;      //互补滤波系数
    float dt;     //两次互补滤波之间的时间间隔 单位S
    float angle;  //互补滤波计算出的角度
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
  * @date     2019/6/18 星期二
  */
void Test_ComplementaryFilter(void);


#endif
