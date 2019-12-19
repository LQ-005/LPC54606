#ifndef __AHRS_H
#define __AHRS_H
#include "Inertial_Sensor.h"
typedef struct 
{
	float q0;
	float q1;
	float q2;
	float q3;
}Q4_t;

typedef struct
{
	vector3f_t a;
	vector3f_t b;
	vector3f_t c;
}Matrix_t;


typedef struct
{
	void (* update)(void);

}AHRS_t;

extern AHRS_t AHRS;

void Test_ahrs(void);    //≤‚ ‘◊ÀÃ¨Ω‚À„

#endif //__AHRS_H

