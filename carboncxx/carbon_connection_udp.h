
#ifndef CARBONCXX_CARBON_CONNECTION_UDP_H_
#define CARBONCXX_CARBON_CONNECTION_UDP_H_

#include <boost/asio/ip/udp.hpp>
#include "carbon_connection.h"

class CarbonConnectionUDP: public CarbonConnection {

public:

    /**
     * Create a Carbon connection using an underlying udp socket.
     * @param host ip of the carbon cache or relay
     * @param port of the plain line receiver of the cache/relay
     * @param io_bservice
     */
    CarbonConnectionUDP(const std::string & host, int port,
                       boost::asio::io_service & io_bservice);

    virtual ~CarbonConnectionUDP();

    /**
     * Send a string line throw the wire.
     * @param line it must have the "\n" at the end
     */
    virtual void sendLine(const std::string & line);

    /**
     * open the udp socket.
     */
    virtual void connect();

    /**
     * close the udp socket.
     */
    virtual void disconnect();

    virtual bool is_connected();

protected:

    boost::asio::ip::udp::socket socket_;

    bool connected_;

};

#endif /* CARBONCXX_CARBON_CONNECTION_UDP_H_ */
