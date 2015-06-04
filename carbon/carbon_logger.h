/*
 * carbon_logger.h
 *
 *  Created on: 01/06/2015
 *      Author: pablin
 */

#ifndef CARBON_LOGGER_H_
#define CARBON_LOGGER_H_

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>

#include "carbon_connection.h"
#include "carbon_metric.h"

class CarbonLogger {

public:

    CarbonLogger(const std::string & prefix,
                  std::vector<std::shared_ptr<CarbonConnection>> cons,
                  float dump_interval = 1.0,
                  int precission = 1000);

    virtual ~CarbonLogger();

    void log_hit(const std::string& path);

    void log_count(const std::string& path, double count);

    void log_duration(const std::string& path, double timelapse);

    void run_dumping_thread();

    void stop_dumping_thread();

private:

    void do_dump();

    void send_to_carbon(const std::string & line);

    std::shared_ptr<CarbonMetric> get_metric(const std::string & metric);

    typedef std::map<std::string, std::shared_ptr<CarbonMetric>> MetricMap;

    std::vector<std::shared_ptr<CarbonConnection>> carbon_connections_;

    std::mutex metrics_mutex_;

    MetricMap metrics_;

    //thread_local needs gcc 4.8.0 or higgher
    static thread_local MetricMap metrics_cache_;
    //MetricMap metrics_cache_;

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
