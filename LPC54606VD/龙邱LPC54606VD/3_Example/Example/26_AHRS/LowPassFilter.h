#ifndef __FILTER_H
#define __FILTER_H

#include "fsl_common.h"


/* 滤波器结构体 */
typedef struct
{
	float sample_freq;     //输入信号频率
    float cutoff_freq;     //截止频率
    float alpha;           //系数
    float oupt;            //低通滤波器输出
}filter_t;


typedef struct
{
    void (* init)(filter_t *filter, float sample_freq, float cutoff_freq);
	float (* apply)(filter_t *filter, float sample) ;
}low_filter_t;


extern low_filter_t low_pass_filter;

#endif //__INERTIAL_H

