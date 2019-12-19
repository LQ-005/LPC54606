#ifndef __AP_MATH_H
#define __AP_MATH_H
#include "fsl_common.h"
#define M_2PI 6.283185307179586f
# define M_PI 3.141592653589793f
#define M_PI_2 1.570796326794897f

#define DEG_TO_RAD 0.017453292519943295769236907684886f
#define RAD_TO_DEG 57.295779513082320876798154814105f

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

float invSqrt(float x); 
float fast_atan(float v);
float constrain_float(float amt, float low, float high);
int16_t constrain_int16(int16_t amt, int16_t low, int16_t high);
float wrap_180_cd(float error);
/*
  wrap an angle in centi-degrees to -90..90
 */
float wrap_90_cd(float error);
#endif //

