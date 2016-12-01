
#include "signal.h"
#include "boost/asio/io_service.hpp"
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include <boost/bind.hpp>

#include "carboncxx/carbon_logger.h"
#include "carboncxx/carbon_connection_tcp.h"
#include "carboncxx/carbon_connection_udp.h"

volatile std::atomic_bool flag;

void sigint(int a)
{
    flag.store(true);
}

int main(int argc, char **argv)
{
    flag.store(false);
    signal(SIGINT, sigint);

    namespace po = boost::program_options;

    po::variables_map vm;
    po::options_description desc("Options");
    desc.add_options()
         ("help,h", "produce help messages")
         ("carbon-host,c", po::value<std::string>()
                    ->default_value("127.0.0.1"),
                    "ip of the carbon cache")
         ("carbon-port,p", po::value<int>()
                    ->default_value(2003),
                    "Port of carbon cache")
         ("udp,u", "Set to use udp isntead of tcp");
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    // The library use boost for the network connections
    boost::asio::io_service ios;

    // Create carbon connection
    std::string carbon_host = vm["carbon-host"].as<std::string>();
    int carbon_port = vm["carbon-port"].as<int>();
    std::vector<std::shared_ptr<CarbonConnection>> cons;
    if (vm.count("udp")){
        cons.push_back(std::make_shared<CarbonConnectionUDP>(carbon_host,
                       carbon_port, ios));
    } else {
        cons.push_back(std::make_shared<CarbonConnectionTCP>(carbon_host,
                       carbon_port, ios));
    }

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
