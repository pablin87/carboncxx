
#ifndef MOCK_CARBON_CONNECTION_H_
#define MOCK_CARBON_CONNECTION_H_

#include <boost/asio/io_service.hpp>
#include "carboncxx/carbon_connection_tcp.h"

class MockCarbonConnection: public CarbonConnectionTCP {

public:

    MockCarbonConnection();

    virtual ~MockCarbonConnection();

    /**
     * Sabe the last line argument call to the internal line string.
     * @param line
     */
    virtual void sendLine(const std::string & line);

    /**
     * do nothing
     */
    virtual void connect();

    /**
     * do nothing
     */
    virtual void disconnect();

    /**
     * Always return true
     * @return true
     */
    virtual bool is_connected();

    std::string line_;

    static boost::asio::io_service ios_;
};

#endif /* MOCK_CARBON_CONNECTION_H_*/
