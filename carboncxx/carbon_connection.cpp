#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include "carbon_connection.h"

CarbonConnection::CarbonConnection(const std::string & host, int port,
        boost::asio::io_service & io_bservice):
        ip_(host), port_(port), io_bservice_(io_bservice)
{

}

CarbonConnection::~CarbonConnection()
{

}

std::string CarbonConnection::fancy_name()
{
    std::ostringstream ss;
    ss << ip_ << ":" << port_;
    return ss.str();
}
