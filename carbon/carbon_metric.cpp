/*
 * carbon_metric.cpp
 *
 *  Created on: 01/06/2015
 *      Author: pablin
 */

#include "carbon_metric.h"
#include <cmath>

CarbonMetric::CarbonMetric(const std::string & path, int precission)
:path_(path), precission_(precission)
{
    time_since_epoc_ = std::chrono::system_clock::now().time_since_epoch();
}

CarbonMetric::~CarbonMetric() { }

void CarbonMetric::increase(double value)
{
    long new_val = std::round(value * precission_);
    value_.fetch_add(new_val);
}

CarbonMetric::MetricData
CarbonMetric::get_and_reset()
{
    // set the value to 0 and return last value.
    long val = value_.load(std::memory_order_relaxed);
    while( !value_.compare_exchange_weak(val, 0,
                                         std::memory_order_release,
                                         std::memory_order_relaxed));
    MetricData data;
    data.timestamp = time_since_epoc_;
    time_since_epoc_ = std::chrono::system_clock::now().time_since_epoch();
    data.value = double(val) / double(precission_);
    return data;
}
