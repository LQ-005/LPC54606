#ifndef __INERTIAL_SENSOR_H
#define __INERTIAL_SENSOR_H
#include "fsl_common.h"

typedef struct
{
	float x;
	float y;
	float z;
}vector3f_t;

typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
}vector3i_t;


typedef struct
{
	uint8_t (* init)(void);
	void (* calibration)(void);
	void (* update)(void);
	vector3f_t (* get_gyro)(void);
	vector3f_t (* get_acc)(void);
}ins_t;

extern ins_t ins;

/* ȫ�ֱ��� ��ų��������ļ��ٶȼ� ���������� */
extern vector3f_t gyro_vector;
extern vector3f_t acc_vector;
extern uint8_t imu_type;


#endif //__INERTIAL_H

