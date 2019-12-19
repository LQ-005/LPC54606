#include "AP_Math.h"
#include "LowPassFilter.h"


/**设置低通滤波的频率sample_freq  和  截止频率cutoff_freq*/
void LowPassFilter_Init(filter_t *filter, float sample_freq, float cutoff_freq)
{
    filter->sample_freq = sample_freq;
    filter->cutoff_freq = cutoff_freq;
    
    if (filter->cutoff_freq <= 0.0f || filter->sample_freq <= 0.0f) {
        filter->alpha = 1.0;
    } else {
        float dt = 1.0f/filter->sample_freq;
        float rc = 1.0f/(M_2PI*filter->cutoff_freq);
        filter->alpha = constrain_float(dt/(dt+rc), 0.0f, 1.0f);
    }
}

/*需要滤波的信号sample  上次的输出信号_output*/
float LowPassFilter_apply(filter_t *filter, float sample) 
{
    filter->oupt += (sample - filter->oupt) * filter->alpha;
    return filter->oupt;
}


low_filter_t low_pass_filter = 
{
    LowPassFilter_Init,
    
    LowPassFilter_apply,
};
