/*
 * simple_example.cpp
 *
 *  Created on: 05/06/2015
 *      Author: pablin
 */

#include "signal.h"
#include "boost/asio/io_service.hpp"

#include "carbon/carbon_logger.h"

volatile std::atomic_bool flag;

void sigint(int a)
{
    flag = true;
}

int main()
{
    flag = false;
    signal(SIGINT, sigint);
    boost::asio::io_service ios;
    std::vector<std::shared_ptr<CarbonConnection>> cons;
    cons.push_back(std::make_shared<CarbonConnection>("127.0.0.1", 2003, ios));
    CarbonLogger clog("prefix", cons);
    clog.init();
    std::chrono::milliseconds loop_interval(250);

    std::thread t([&](){
        ios.run();
    });

    std::cout << "Started" << std::endl;
    while(!flag){
        clog.log_hit("hit");
        clog.log_count("count", 5);
        std::this_thread::sleep_for(loop_interval);
    }
    ios.stop();
    t.join();
    std::cout << "Stopped" << std::endl;

    return 0;
}
