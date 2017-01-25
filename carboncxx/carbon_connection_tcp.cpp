
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "carbon_connection_tcp.h"

CarbonConnectionTCP::CarbonConnectionTCP(const std::string & host, int port,
        boost::asio::io_service & io_bservice):
        CarbonConnection(host, port, io_bservice),
        socket_(io_bservice), connected_(false)
{ }

CarbonConnectionTCP::~CarbonConnectionTCP()
{
    // try to close socket
    disconnect();
}

void CarbonConnectionTCP::sendLine(const std::string & line)
{
    namespace ba = boost::asio;
    boost::system::error_code ec;
    ba::write(socket_, ba::buffer(line.c_str(), line.length()), ec);
    if (ec){
        std::ostringstream ss;
        ss << "Error when sending data to carbon " << fancy_name()
            << " : " << ec.message() << " (" << ec.value() << ")";
        throw std::runtime_error( ss.str());
    }
}

boost::asio::ip::tcp::endpoint CarbonConnectionTCP::
resolve_endpoint(const std::string & host, int port)
{
	std::string str_port = std::to_string(port);
	boost::asio::ip::tcp::resolver resolver(io_bservice_);
	boost::asio::ip::tcp::resolver::query query(host, str_port);
	boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);

	return iter->endpoint();
}

void CarbonConnectionTCP::connect()
{
    using namespace boost::asio::ip;

    tcp::endpoint endpoint = resolve_endpoint(host_, port_);
    boost::system::error_code ec;
    socket_.connect(endpoint, ec);
    if (ec){
        std::ostringstream ss;
        ss << "Error when connecting to carbon " << fancy_name()
           << " : " << ec.message() << "(" << ec.value() << ")";
        throw std::runtime_error(ss.str());
    }
    connected_ = true;
}

void CarbonConnectionTCP::disconnect()
{
    if ( connected_ ) socket_.close();
    connected_ = false;
}


bool CarbonConnectionTCP::is_connected()
{
    return connected_;
}
