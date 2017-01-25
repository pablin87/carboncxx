
#ifndef CARBON_CONNECTION_TCP_H_
#define CARBON_CONNECTION_TCP_H_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <iostream>

#include "carbon_connection.h"

class CarbonConnectionTCP : public CarbonConnection {

public:

    /**
     * Create a Carbon connection using an underlying tcp socket.
     * @param host ip of the carbon cache or relay
     * @param port of the plain line receiver of the cache/relay
     * @param io_bservice
     */
    CarbonConnectionTCP(const std::string & host, int port,
                       boost::asio::io_service & io_bservice);

    virtual ~CarbonConnectionTCP();

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

    virtual bool is_connected();

protected:

    /**
     * Helpper function to resolve host/ip  port to and endpoint
     */
    boost::asio::ip::tcp::endpoint
    resolve_endpoint(const std::string & host, int port);

    boost::asio::ip::tcp::socket socket_;

    bool connected_;
};

#endif /* CARBON_CONNECTION_TCP_H_ */
