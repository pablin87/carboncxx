/*
 * carbon_connection.h
 *
 *  Created on: 01/06/2015
 *      Author: pablin
 */

#ifndef CARBON_CONNECTION_H_
#define CARBON_CONNECTION_H_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <iostream>

class CarbonConnection {

public:

    CarbonConnection(const std::string & host, int port,
                       boost::asio::io_service & io_bservice);

    virtual ~CarbonConnection();

    virtual void sendLine(const std::string & line);

    virtual void connect();

    virtual void disconnect();

    virtual bool is_connected();

private:

    std::string ip_;

    int port_;

    boost::asio::io_service & io_bservice_;

    boost::asio::ip::tcp::socket socket_;

    bool connected_;
};

#endif /* CARBON_CONNECTION_H_ */
