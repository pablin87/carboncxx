
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>

#include "carbon_connection_udp.h"


CarbonConnectionUDP::CarbonConnectionUDP(const std::string & host, int port,
        boost::asio::io_service & io_bservice):
        CarbonConnection(host, port, io_bservice),
        socket_(io_bservice), connected_(false)
{ }

CarbonConnectionUDP::~CarbonConnectionUDP()
{
    // try to close socket
    disconnect();
}

void CarbonConnectionUDP::sendLine(const std::string & line)
{
    namespace ba = boost::asio;

    socket_.send_to(ba::buffer(line.c_str(), line.length()), endpoint_);
}

void CarbonConnectionUDP::connect()
{
    using namespace boost::asio::ip;

    udp::endpoint endpoint( address::from_string(ip_), port_);
    endpoint_ = endpoint;
    boost::system::error_code ec;
    socket_.open(udp::v4(), ec);

    if (ec){
        std::ostringstream ss;
        ss << "Error when connecting to carbon " << fancy_name()
           << " : " << ec.message() << "(" << ec.value() << ")";
        throw std::runtime_error(ss.str());
    }
    connected_ = true;
}

void CarbonConnectionUDP::disconnect()
{
    if ( connected_ ) socket_.close();
    connected_ = false;
}


bool CarbonConnectionUDP::is_connected()
{
    return connected_;
}
