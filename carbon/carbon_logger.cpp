/*
 * carbon_logger.cpp
 *
 *  Created on: 01/06/2015
 *      Author: pablin
 */

#include "carbon_logger.h"
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <string>

CarbonLogger::CarbonLogger(const std::string & prefix,
        std::vector<std::shared_ptr<CarbonConnection>> cons,
        float dump_interval, int precission)
:carbon_connections_(cons), dump_interval_(dump_interval),
 precission_(precission), continue_dumping_(false), prefix_(prefix)
{

}

CarbonLogger::~CarbonLogger() { }

std::shared_ptr<CarbonMetric>
CarbonLogger::get_metric(const std::string & metric)
{
    std::shared_ptr<CarbonMetric> ret(NULL);

    // First, check if we have the reference in the local thread cache
    auto it_local = metrics_cache_.find(metric);
    if (it_local != metrics_cache_.end()) {
        // We found the metric in the local thread cache. Just return it.
        ret = *it_local;
    } else {
        // We do not have the metric in the local thread cache. Check if it
        // exists in the global metric map.
        {
            std::lock_guard lock(metrics_mutex_);
            auto it_global = metrics_.find(metric);
            if (it_global != metrics_.end()){
                // The metric already exists in the global map. Then, add it to
                // the local thread cache and return it.
                metrics_cache_[metric] = *it_global;
                ret = *it_global;
            } else {
                // The metric does not exist in the global metrics. Then add the
                // metric to the global and local map and return it.
                metrics_[metric] = std::make_shared<CarbonMetric>(metric,
                                                                  precission_);
                metrics_cache_[metric] = metrics_[metric];
                {
                    std::lock_guard(all_metrics_mutex_);
                    all_metrics_.push_back(metric);
                }
            }
        } // unlock mutex
    }

    return ret;
}

void
CarbonLogger::log_hit(const std::string& path)
{
    log_count(path, 1.0);
}

void
CarbonLogger::log_count(const std::string& path, double count)
{
    std::shared_ptr<CarbonMetric> metric = get_metric(path);
    metric->increase(count);
}

void
CarbonLogger::log_duration(const std::string& path, double timelapse)
{
    log_count(path, timelapse);
}

void
CarbonLogger::do_dump()
{
    // Get all metrics...
    all_metrics_mutex_.lock();
    std::vector<std::string> all_metrics(all_metrics_);
    all_metrics_mutex_.unlock();

    // Now hit carbon with values.
    for ( const std::string& path : all_metrics){
        std::shared_ptr<CarbonMetric> metric = get_metric(path);
        CarbonMetric::MetricData metric_data = metric->get_and_reset();
        std::stringstream ss;
        ss << prefix_ << "." << path << "\t" << metric_data.value
           << "\t" << metric_data.timestamp;
        send_to_carbon(ss.str());
    }
}

void
CarbonLogger::send_to_carbon(const std::string & line)
{
    int instance = std::rand() % carbon_connections_.size();
    carbon_connections_[instance]->sendLine(line);
}

void
CarbonLogger::run_dumping_thread()
{
    dump_thread_ = std::make_shared<std::thread>([this]{
        continue_dumping_ = true;
        int tmp_milliseconds = dump_interval_ * 1000;
        std::chrono::milliseconds interval(tmp_milliseconds);
        std::chrono::milliseconds loop_interval(100);

        while(continue_dumping_){
            auto start = std::chrono::high_resolution_clock::now();
            std::chrono::duration elapsed(0);

            // Little hack to check the continue_dumping_ flag more often.
            do {
                std::this_thread::sleep_for(loop_interval);
                auto end = std::chrono::high_resolution_clock::now();
                elapsed = end - start;
            } while(elapsed <= interval && continue_dumping_);

            if (continue_dumping_) do_dump();

        }

    });
}

void
CarbonLogger::stop_dumping_thread()
{
    continue_dumping_ = false;
    dump_thread_->join();
}