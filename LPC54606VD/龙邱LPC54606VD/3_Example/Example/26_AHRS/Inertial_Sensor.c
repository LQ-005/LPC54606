#include "Inertial_Sensor.h"
#include "AP_Math.h"
#include "LowPassFilter.h"
#include "LQ_9AX.h"
#include "LQ_MPU6050.h"
#include "LQ_LED.h"
#include "LQ_ICM20602.h"
#include "config.h"
/* 声明外部延时函数 */
extern void delayms(uint16_t ms);


#define G		    9.80665f		        // m/s^2	
#define RadtoDeg    57.324841f				//弧度到角度 (弧度 * 180/3.1415)
#define DegtoRad    0.0174533f				//角度到弧度 (角度 * 3.1415/180)
#define Acc_Gain  	0.0001220f				//加速度变成G (初始化加速度满量程-+4g LSBa = 2*4/65535.0)
#define Gyro_Gain 	0.0609756f				//角速度变成度 (初始化陀螺仪满量程+-2000 LSBg = 2*2000/65535.0)
#define Gyro_Gr	    0.0010641f			    //角速度变成弧度(3.1415/180 * LSBg)   
#define VAR         0.1f                    //1/方差

/* 低通滤波器参数 */
filter_t  low_filter_acc_x;
filter_t  low_filter_acc_y;
filter_t  low_filter_acc_z;
filter_t  low_filter_gyro_x;
filter_t  low_filter_gyro_y;
filter_t  low_filter_gyro_z;

/* 传感器类型 */
uint8_t imu_type;

/**
  * @brief    惯性传感器初始化
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 星期一
  */
uint8_t ins_init(void)
{
    /* 初始化低通滤波器 */    
    low_pass_filter.init(&low_filter_acc_x, 1000, 20);
    low_pass_filter.init(&low_filter_acc_y, 1000, 20);
    low_pass_filter.init(&low_filter_acc_z, 1000, 20);
    
    low_pass_filter.init(&low_filter_gyro_x, 1000, 20);
    low_pass_filter.init(&low_filter_gyro_y, 1000, 20);
    low_pass_filter.init(&low_filter_gyro_z, 1000, 20);
#ifdef IMU_USE_SELECT
    /* 使用MPU6050 或者ICM20602 IIC时 */
    if(MPU6050_Init() == 0)
    {
        imu_type = 1;
        delayms(100);
        return 0;
    }
    /* 使用龙邱九轴 IIC时 */
    else if(LQ9AX_Init() == 0)
    {
        FX_8700_Active(0);
        delayms(10);
        FX_8700_SetRate(800, 0);   // 800Hz  仅加速度计使能
        FX_8700_SetRange(4);       //± 4g
        FX_8700_Active(1);
        
        imu_type = 2;
        delayms(100);
        return 0;
    }
#endif
    
#ifdef ICM_USE_SELECT
    /* 使用ICM20602 SPI时 */
    if(ICM20602_Init() == 0)
    {
        imu_type = 3;
        delayms(100);
        return 0;
    }
#endif
    
    
    
    return 1;
}

/* 传感器校准标志位 */
bool flag_ins_calibration = true;

/*惯性传感器校准*/
vector3f_t acc_vector_offset;   //加计零偏值
vector3f_t gyro_vector_offset;  //角速度计零偏值

/**
  * @brief    传感器校准 以当前静止面为水平面
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 星期一
  */
void ins_calibration(void)
{  
    
    vector3f_t _acc_var;           //存放加计方差
    vector3i_t _acc_vector[100];   //存放加计读取的原始数据
    vector3i_t _gyro_vector[100];  //存放角速度计读取的原始数据
    vector3f_t _gyro_average;      //存放角速度计平均值
    vector3f_t _acc_average;       //存放加计平均值
    vector3f_t _gyro_var;          //存放角速度计方差
    
    do{
		_acc_var.x = 0;
	    _acc_var.y = 0;
	    _acc_var.z = 0;
	    
	    _gyro_average.x = 0;
	    _gyro_average.y = 0;
	    _gyro_average.z = 0;
	    
	    _acc_average.x = 0;
	    _acc_average.y = 0;
	    _acc_average.z = 0;
	    
	    _gyro_var.x = 0;
	    _gyro_var.y = 0;
	    _gyro_var.z = 0;
		
        for(int i = 0; i < 100; i++)
        {
#ifdef IMU_USE_SELECT
            /* 获取传感器原始数据 */
            if(imu_type == 1)
            {
                MPU_Get_Raw_data(&_acc_vector[i].x, &_acc_vector[i].y, &_acc_vector[i].z,
                                 &_gyro_vector[i].x,&_gyro_vector[i].y,&_gyro_vector[i].z);	  
            }
            else if(imu_type == 2)
            {
                FX_8700_GetACCRaw(&_acc_vector[i].x, &_acc_vector[i].y, &_acc_vector[i].z);	  
                FX_21002_GetRaw(&_gyro_vector[i].x, &_gyro_vector[i].y, &_gyro_vector[i].z);
            }
#endif
            
#ifdef ICM_USE_SELECT
            if(imu_type == 3)
            {
                ICM_Get_Raw_data(&_acc_vector[i].x, &_acc_vector[i].y, &_acc_vector[i].z,
                                 &_gyro_vector[i].x,&_gyro_vector[i].y,&_gyro_vector[i].z);	  
            }
#endif   
            _acc_average.x  += _acc_vector[i].x/100.0f;
            _acc_average.y  += _acc_vector[i].y/100.0f;
            _acc_average.z  += (_acc_vector[i].z - 8192)/100.0f; 
            
            _gyro_average.x += _gyro_vector[i].x/100.0f;
            _gyro_average.y += _gyro_vector[i].y/100.0f;
            _gyro_average.z += _gyro_vector[i].z/100.0f;
            
            delayms(2);
            if(i % 20 == 0)
            {
                LED_ColorReverse(red);     //红灯闪烁
            }
        }
        /* 计算方差 确保校准的时候是静止状态的 */
        for(int j = 0; j < 100; j++)
        {
            _acc_var.x +=  0.01f * (_acc_vector[j].x - _acc_average.x) * (_acc_vector[j].x - _acc_average.x);
            _acc_var.y +=  0.01f * (_acc_vector[j].y - _acc_average.y) * (_acc_vector[j].y - _acc_average.y);
            _acc_var.z +=  0.01f * (_acc_vector[j].z - _acc_average.z) * (_acc_vector[j].z - _acc_average.z);
            
            _gyro_var.x +=  0.01f * (_gyro_vector[j].x - _gyro_average.x) * (_gyro_vector[j].x - _gyro_average.x);
            _gyro_var.y +=  0.01f * (_gyro_vector[j].y - _gyro_average.y) * (_gyro_vector[j].y - _gyro_average.y);
            _gyro_var.z +=  0.01f * (_gyro_vector[j].z - _gyro_average.z) * (_gyro_vector[j].z - _gyro_average.z);
        }
        
        /* 快速计算 1/Sqrt(x) */
        _gyro_var.x = invSqrt(_gyro_var.x);
        _gyro_var.y = invSqrt(_gyro_var.y);
        _gyro_var.z = invSqrt(_gyro_var.z);
        
        if(   _gyro_var.x > VAR
           && _gyro_var.y > VAR 
           && _gyro_var.z > VAR )     
        {
            acc_vector_offset.x   = (int16_t)_acc_average.x;          //保存静止时的零偏值
            acc_vector_offset.y   = (int16_t)_acc_average.y;
            acc_vector_offset.z   = (int16_t)_acc_average.z;
            gyro_vector_offset.x  = (int16_t)_gyro_average.x;
            gyro_vector_offset.y  = (int16_t)_gyro_average.y;
            gyro_vector_offset.z  = (int16_t)_gyro_average.z;
            
            flag_ins_calibration = false;                            //校准标志位清零
        }
    }while(flag_ins_calibration);
}

  


/* 全局变量 存放初步处理后的加速度计 陀螺仪数据 */
vector3f_t gyro_vector;     
vector3f_t acc_vector;      


/**
  * @brief    更新传感器数值
  *
  * @param    
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/6/17 星期一
  */
void ins_update(void)
{
    /* 保存最近三次的数据 */
    static vector3i_t gyro_vector_last[3];
    static vector3i_t acc_vector_last[3];
    static uint8_t num = 0;
    if(num > 2) num = 0;
    
    if(flag_ins_calibration)  //如果需要校准
    {
        ins_calibration();    //校准
    }
    
#ifdef IMU_USE_SELECT
    /* 获取传感器原始数据 */
    if(imu_type == 1)
    {
        MPU_Get_Raw_data(&acc_vector_last[num].x, &acc_vector_last[num].y, &acc_vector_last[num].z,
                         &gyro_vector_last[num].x,&gyro_vector_last[num].y,&gyro_vector_last[num].z);	
    }
    else if(imu_type == 2)
    {
        FX_8700_GetACCRaw(&acc_vector_last[num].x, &acc_vector_last[num].y, &acc_vector_last[num].z);	
        FX_21002_GetRaw(&gyro_vector_last[num].x,&gyro_vector_last[num].y,&gyro_vector_last[num].z);     
    }
#endif
    
#ifdef ICM_USE_SELECT   
    if(imu_type == 3)
    {
        ICM_Get_Raw_data(&acc_vector_last[num].x, &acc_vector_last[num].y, &acc_vector_last[num].z,
                         &gyro_vector_last[num].x,&gyro_vector_last[num].y,&gyro_vector_last[num].z);	
    }
#endif  
    /* 去零偏 */
    acc_vector_last[num].x -= acc_vector_offset.x;   
    acc_vector_last[num].y -= acc_vector_offset.y;   
    acc_vector_last[num].z -= acc_vector_offset.z;   
    gyro_vector_last[num].x -= gyro_vector_offset.x; 
    gyro_vector_last[num].y -= gyro_vector_offset.y; 
    gyro_vector_last[num].z -= gyro_vector_offset.z; 
    
    
    /* 平均 低通滤波 */
    acc_vector.x = low_pass_filter.apply(&low_filter_acc_x, (acc_vector_last[0].x + acc_vector_last[1].x + acc_vector_last[2].x)/3);
    acc_vector.y = low_pass_filter.apply(&low_filter_acc_y, (acc_vector_last[0].y + acc_vector_last[1].y + acc_vector_last[2].y)/3);
    acc_vector.z = low_pass_filter.apply(&low_filter_acc_z, (acc_vector_last[0].z + acc_vector_last[1].z + acc_vector_last[2].z)/3);
    
    gyro_vector.x = low_pass_filter.apply(&low_filter_gyro_x, (gyro_vector_last[0].x + gyro_vector_last[1].x + gyro_vector_last[2].x)/3);
    gyro_vector.y = low_pass_filter.apply(&low_filter_gyro_y, (gyro_vector_last[0].y + gyro_vector_last[1].y + gyro_vector_last[2].y)/3);
    gyro_vector.z = low_pass_filter.apply(&low_filter_gyro_z, (gyro_vector_last[0].z + gyro_vector_last[1].z + gyro_vector_last[2].z)/3);
    
//    gyro_vector.x = (gyro_vector_last[0].x + gyro_vector_last[1].x + gyro_vector_last[2].x)/3;
//    gyro_vector.y = (gyro_vector_last[0].y + gyro_vector_last[1].y + gyro_vector_last[2].y)/3;
//    gyro_vector.z = (gyro_vector_last[0].z + gyro_vector_last[1].z + gyro_vector_last[2].z)/3;
//    
    
    //加速度AD值 转换成 米/平方秒 
	acc_vector.x *=  Acc_Gain * G;
	acc_vector.y *=  Acc_Gain * G;
	acc_vector.z *=  Acc_Gain * G;
    
	//陀螺仪AD值 转换成 弧度/秒    
	gyro_vector.x *=  Gyro_Gr;  
	gyro_vector.y *=  Gyro_Gr;
	gyro_vector.z *=  Gyro_Gr;
    
    
    num++;
}

vector3f_t get_gyro()
{    
	return gyro_vector;
}

vector3f_t get_acc()
{
	return acc_vector;
}

/* 定义一个惯性传感器对象 */
ins_t ins =
{

	ins_init,
	ins_calibration,
	ins_update,
	get_gyro,
	get_acc,

};
