
#ifndef CARBONCXX_CARBON_CONNECTION_H_
#define CARBONCXX_CARBON_CONNECTION_H_

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
    virtual void sendLine(const std::string & line) = 0;

    /**
     * Connect to carbon cache.
     */
    virtual void connect() = 0;

    /**
     * Disconnect from carbon cache.
     */
    virtual void disconnect() = 0;

    /**
     * Check if the instance is connected.
     * @return
     */
    virtual bool is_connected() = 0;

    /**
     * return the ip:port of the carbon instance it is connected to.
     */
    std::string fancy_name();

protected:

    std::string ip_;

    int port_;

    boost::asio::io_service & io_bservice_;

};

#endif /* CARBONCXX_CARBON_CONNECTION_H_ */
