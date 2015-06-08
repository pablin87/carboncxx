
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "CARBONMETRIC"
#include <boost/test/unit_test.hpp>

#include "carboncxx/carbon_metric.h"
#include <thread>

BOOST_AUTO_TEST_CASE( test_carbon_metric)
{
    // "lala" and "pepe" are like foo and bar for spanish people :P
    CarbonMetric metric("lala");
    metric.increase(2.0);

    CarbonMetric::MetricData data = metric.get_and_reset();
    BOOST_CHECK_EQUAL(data.value, 2.0);

    data = metric.get_and_reset();
    BOOST_CHECK_EQUAL(data.value, 0);
}

BOOST_AUTO_TEST_CASE( test_carbon_metric_atomicity)
{
    // "lala" and "pepe" are like foo and bar for spanish people :P
    std::shared_ptr<CarbonMetric> metric( new CarbonMetric("lala"));

    std::list<std::thread> threads;
    for ( int i=0 ; i<8 ; ++i){
        threads.push_back(std::thread([=]{
            for ( int j=0 ; j<10000 ; j++){
                metric->increase(1);
            }
        }));
    }

    for ( auto & t : threads)
        t.join();

    CarbonMetric::MetricData data = metric->get_and_reset();
    BOOST_CHECK_EQUAL(data.value, 80000);
}


