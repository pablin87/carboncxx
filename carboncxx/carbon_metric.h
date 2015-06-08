
#ifndef CARBON_METRIC_H_
#define CARBON_METRIC_H_

#include <atomic>
#include <chrono>
#include <string>
#include <tuple>

class CarbonMetric {

public:

    /**
     * Creates a thread safe, lock free metric for the given path.
     * @param path of the metric
     * @param precission. As the class internally uses an int to save the
     * metric for thread safe reasons, we need to set the precission for the
     * given metric. If precission is 1000, it means that the metric can save
     * values with min values of 0,001. if for example precission is 1, the
     * min value would be 1.
     */
    CarbonMetric(const std::string & path, int precission=1000);

    virtual ~CarbonMetric();

    /**
     * Thread safe and lock-free, it increased the internal counter of the
     * metric.
     */
    void increase(double value = 1.0);

    class MetricData{
    public:
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
