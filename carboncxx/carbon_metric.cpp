
#include "carbon_metric.h"

#include <cmath>

CarbonMetric::CarbonMetric(const std::string & path, int precission)
:path_(path), value_(0), sum_(0), precission_(precission), averaged(false)
{
    time_since_epoc_ = std::chrono::system_clock::now().time_since_epoch();
}

CarbonMetric::~CarbonMetric() { }

void CarbonMetric::increase(double value)
{
    long new_val = std::round(value * precission_);
    value_.fetch_add(new_val);
}

void CarbonMetric::average(double value){
    averaged = true;
    increase(1.0);
    long new_sum_ = std::round(value * precission_);
    sum_.fetch_add(new_sum_);
}

CarbonMetric::MetricData
CarbonMetric::get_and_reset()
{
    // set the value to 0 and return last value.
    MetricData data;
    long val, sum;
    do {
        val = value_.load(std::memory_order_relaxed);
    } while( !value_.compare_exchange_weak(val, 0,
                                           std::memory_order_release,
                                           std::memory_order_relaxed));

    if(averaged){
        do {
            sum = sum_.load(std::memory_order_relaxed);
        } while( !sum_.compare_exchange_weak(sum, 0,
                                               std::memory_order_release,
                                               std::memory_order_relaxed));
        data.value = double(sum) / double(val);
    }else{
        data.value = double(val) / double(precission_);
    }
    
    data.timestamp = time_since_epoc_;
    time_since_epoc_ = std::chrono::system_clock::now().time_since_epoch();
    return data;
}
