/*
 * MockCarbonConnection.h
 *
 *  Created on: 05/06/2015
 *      Author: pablin
 */

#ifndef MOCK_CARBON_CONNECTION_H_
#define MOCK_CARBON_CONNECTION_H_

#include "carbon/carbon_connection.h"
#include <boost/asio/io_service.hpp>

class MockCarbonConnection: public CarbonConnection {

public:

    MockCarbonConnection();

    virtual ~MockCarbonConnection();

    virtual void sendLine(const std::string & line);

    virtual void connect();

    virtual void disconnect();

    virtual bool is_connected();

    std::string line_;

    static boost::asio::io_service ios_;
};

#endif /* MOCK_CARBON_CONNECTION_H_*/
