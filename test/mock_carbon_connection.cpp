/*
 * MockCarbonConnection.cpp
 *
 *  Created on: 05/06/2015
 *      Author: pablin
 */

#include "mock_carbon_connection.h"

boost::asio::io_service MockCarbonConnection::ios_;

MockCarbonConnection::MockCarbonConnection()
:CarbonConnection("", 0, ios_) { }

MockCarbonConnection::~MockCarbonConnection() { }

void
MockCarbonConnection::sendLine(const std::string & line)
{
    line_ = line;
}


// Don nothing when connecting/disconnecting
void MockCarbonConnection::connect() {}

void MockCarbonConnection::disconnect() {}

bool MockCarbonConnection::is_connected()
{
    return true;
}
