
#ifndef CARBON_CONNECTION_H_
#define CARBON_CONNECTION_H_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <iostream>

class CarbonConnection {

public:

    /**
     * Create a Carbon connection
     * @param host ip of the carbon cache or relay
     * @param port of the plain line receiver of the cache/relay
     * @param io_bservice
     */
    CarbonConnection(const std::string & host, int port,
                       boost::asio::io_service & io_bservice);

    virtual ~CarbonConnection();

    /**
     * Send a string line throw the wire.
     * @param line it must have the "\n" at the end
     */
    virtual void sendLine(const std::string & line);

    /**
     * Do the tcp connect.
     */
    virtual void connect();

    /**
     * Do the tcp disconnect (aka close the socket).
     */
    virtual void disconnect();

    /**
     * Check if the instance is connected.
     * @return
     */
    virtual bool is_connected();

private:

    std::string ip_;

    int port_;

    boost::asio::io_service & io_bservice_;

    boost::asio::ip::tcp::socket socket_;

    bool connected_;
};

#endif /* CARBON_CONNECTION_H_ */
