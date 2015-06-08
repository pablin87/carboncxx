
#ifndef CARBON_LOGGER_H_
#define CARBON_LOGGER_H_

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>

#include "boost/thread.hpp"

#include "carbon_connection.h"
#include "carbon_metric.h"

class CarbonLogger {

public:

    /**
     * Creates an instance of the Carbon Logger. This should be created one
     * per process, although nothing stops you from creating more.
     * @param prefix to prepend to all metrics that will be logged
     * @param cons connections to the different carbon caches/relays
     * @param dump_interval interval that used by the dumping thread. It will
     * hit graphite for each metric using this time interval (in seconds).
     * @param precission for all the logging metrics. 1000 says that the
     * metrics will have 1/1000 units of precission.
     */
    CarbonLogger(const std::string & prefix,
                  std::vector<std::shared_ptr<CarbonConnection>> cons,
                  float dump_interval = 1.0,
                  int precission = 1000);

    /**
     * Move contructor.
     * @param cl
     */
    CarbonLogger( CarbonLogger && cl);

    /**
     * Safely call connect to all the connections that the logger has.
     */
    void init();

    /**
     * Safely call disconnect to all the connections that the logger has.
     */
    virtual ~CarbonLogger();

    /*
     * All the log_* metrics are THREAD SAFE, and for each thread, except for
     * the first call for each metric, all subsequents calls are lock-free
     * (first ones needs a lock acquire ).
     */

    /**
     * Log a hit for the given metric. Same as log_count with a "1".
     * @param path metric name
     */
    void log_hit(const std::string& path);

    /**
     * Log the given amount to the given path.
     * @param path metric name
     * @param count amount to be logged for the metric
     */
    void log_count(const std::string& path, double count);

    /**
     *
     * @param path metric name
     * @param timelapse time to be logged for the given metric
     */
    void log_duration(const std::string& path, double timelapse);

    /**
     * Starts a "infinite" loop to dump all the metrics.
     * Internally it will, iterate throw all metrics, send the proper
     * values to the carbon connections and reset all the metrics.
     */
    void run_dumping_thread();

    /**
     * Stop the dumping thread. (can take some milliseconds at it does a join).
     */
    void stop_dumping_thread();

    /**
     * Iterate over all metrics, dump them to the carbon relays/caches and
     * reset the metrics.
     * This is thread safe, and it is public for testing purposes.
     * Although this method can be used to forced the dump of the metrics,
     * using it when you are already using the run_dumping_thread has no sense.
     */
    void do_dump();

private:

    /**
     * Pick a carbon connection (randomly) and send the line to it.
     * @param line
     */
    void send_to_carbon(const std::string & line);

    /**
     * Given a metric path, it returns the corresponding metric, using the
     * thread-cache metrics if possible. Also, if the metric does not exist,
     * the metric will be created.
     * This method is THREAD SAFE.
     * @param metric path without prefix
     * @return Specific CarbonMetric corresponding to the given metric
     */
    std::shared_ptr<CarbonMetric> get_metric(const std::string & metric);

    typedef std::map<std::string, std::shared_ptr<CarbonMetric>> MetricMap;

    /**
     * Get the metric cache corresponding to the actual thread. If it does not
     * exist, it creates one.
     * @return
     */
    MetricMap & get_metrics_cache();

    std::vector<std::shared_ptr<CarbonConnection>> carbon_connections_;

    std::mutex metrics_mutex_;

    MetricMap metrics_;

    //thread_local needs gcc 4.8.0 or higgher

    boost::thread_specific_ptr<MetricMap> metrics_cache_;

    float dump_interval_;

    int precission_;

    volatile bool continue_dumping_;

    // used by dumping thread (optimization).
    std::mutex all_metrics_mutex_;
    std::vector<std::string> all_metrics_;
    std::shared_ptr<std::thread> dump_thread_;

    std::string prefix_;

};

#endif /* CARBON_LOGGER_H_ */
