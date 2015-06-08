
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "CARBONLOGGER"

#include <thread>
#include <tuple>

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>

#include "carboncxx/carbon_logger.h"
#include "mock_carbon_connection.h"

CarbonLogger
create_mock_carbon_logger(const std::string & pref,
                             std::shared_ptr<MockCarbonConnection> mock_con)
{
    std::vector<std::shared_ptr<CarbonConnection>> cons;
    cons.push_back(mock_con);
    CarbonLogger clog(pref, cons);
    return std::move(clog);
}

void check_carbon_line(const std::string & line,
                          const std::string & exp_path, double exp_value)
{
    std::vector<std::string> strs;
    boost::split(strs, line, boost::is_any_of(" "));

    BOOST_CHECK_EQUAL(strs.size(), 3);
    BOOST_CHECK_EQUAL(exp_path, strs[0]);
    BOOST_CHECK_EQUAL(exp_value, std::stod(strs[1]));
}

BOOST_AUTO_TEST_CASE( test_carbon_logger)
{
    std::shared_ptr<MockCarbonConnection> mock_con(new MockCarbonConnection());
    CarbonLogger clog = create_mock_carbon_logger("prefix", mock_con);
    clog.log_hit("pepe");
    clog.do_dump();

    check_carbon_line(mock_con->line_, "prefix.pepe", 1);}

BOOST_AUTO_TEST_CASE( test_carbon_logger_value )
{
    std::shared_ptr<MockCarbonConnection> mock_con(new MockCarbonConnection());
    CarbonLogger clog = create_mock_carbon_logger("", mock_con);
    double count = 5;
    clog.log_count("pepe", count);
    clog.log_count("pepe", count);
    clog.do_dump();

    check_carbon_line(mock_con->line_, "pepe", count * 2);
}


