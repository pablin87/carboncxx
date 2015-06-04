/*
 * carbon_metric.h
 *
 *  Created on: 01/06/2015
 *      Author: pablin
 */

#ifndef CARBON_METRIC_H_
#define CARBON_METRIC_H_

#include <atomic>
#include <chrono>
#include <string>
#include <tuple>

class CarbonMetric {

public:

    CarbonMetric(const std::string & path, int precission=1000);

    virtual ~CarbonMetric();

    /**
     * Thread safe and lock-free, it increased the internal counter of the
     * metric.
     */
    void increase(double value = 1.0);

    class MetricData{
        double value;
        std::chrono::duration<double> timestamp;
    };

    /**
     * Thread safe and lock free, it reset the internal counter to 0 and
     * retrieves the value of the counter.
     */
    MetricData get_and_reset();

private:

    std::string path_;

    std::atomic_long value_;

    const int precission_;

    std::chrono::duration<double> time_since_epoc_;

};

#endif /* CARBON_METRIC_H_ */
