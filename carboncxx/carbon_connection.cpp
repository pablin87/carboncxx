
#include "carbon_connection.h"

CarbonConnection::CarbonConnection(const std::string & host, int port,
        boost::asio::io_service & io_bservice):
        ip_(host), port_(port), io_bservice_(io_bservice),
        socket_(io_bservice), connected_(false)
{

}

CarbonConnection::~CarbonConnection()
{
    // try to close socket
    disconnect();
}


void CarbonConnection::sendLine(const std::string & line)
{
    boost::system::error_code ec;
    socket_.write_some(boost::asio::buffer(line.c_str(), line.size()));
    if (ec){
        throw std::runtime_error("Error when sending data to carbon" +
                                 ec.message());
    }
}

void CarbonConnection::connect()
{
    using namespace boost::asio::ip;

    tcp::endpoint endpoint( address::from_string(ip_), port_);
    boost::system::error_code ec;

    socket_.connect(endpoint, ec);

    if (ec)
    {
      throw std::runtime_error("Error when connecting to carbon " +
                                ec.message());
    }
    connected_ = true;

}

void CarbonConnection::disconnect()
{
    if ( connected_ ) socket_.close();
    connected_ = false;
}


bool CarbonConnection::is_connected()
{
    return connected_;
}
