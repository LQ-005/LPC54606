#ifndef __FILTER_H
#define __FILTER_H

#include "fsl_common.h"


/* �˲����ṹ�� */
typedef struct
{
	float sample_freq;     //�����ź�Ƶ��
    float cutoff_freq;     //��ֹƵ��
    float alpha;           //ϵ��
    float oupt;            //��ͨ�˲������
}filter_t;


typedef struct
{
    void (* init)(filter_t *filter, float sample_freq, float cutoff_freq);
	float (* apply)(filter_t *filter, float sample) ;
}low_filter_t;


extern low_filter_t low_pass_filter;

#endif //__INERTIAL_H

