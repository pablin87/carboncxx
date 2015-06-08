
#include "signal.h"
#include "boost/asio/io_service.hpp"

#include "carboncxx/carbon_logger.h"

volatile std::atomic_bool flag;

void sigint(int a)
{
    flag = true;
}

int main()
{
    flag = false;
    signal(SIGINT, sigint);

    // The library use boost for the network connections
    boost::asio::io_service ios;

    // Create carbon connection
    std::vector<std::shared_ptr<CarbonConnection>> cons;
    cons.push_back(std::make_shared<CarbonConnection>("127.0.0.1", 2003, ios));
    CarbonLogger clog("prefix", cons);

    // Start the carbon logger thread
    clog.init();
    clog.run_dumping_thread();

    // And we are now ready to log stats
    std::cout << "Started" << std::endl;
    std::chrono::milliseconds loop_interval(100);
    while(!flag){
        clog.log_hit("hit");
        clog.log_count("count", 5);
        std::this_thread::sleep_for(loop_interval);
    }

    // Exit cleanly, stoping the carbon logger
    clog.stop_dumping_thread();

    std::cout << "Stopped" << std::endl;

    return 0;
}
