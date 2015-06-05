
ADD_EXECUTABLE(test_carbon_metric test_carbon_metric)
TARGET_LINK_LIBRARIES(test_carbon_metric
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      carboncxx )
ADD_TEST(test_carbon_metric test_carbon_metric COMMAND test_carbon_metric )

ADD_EXECUTABLE(test_carbon_logger test_carbon_logger mock_carbon_connection)
TARGET_LINK_LIBRARIES(test_carbon_logger
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      carboncxx )
ADD_TEST(test_carbon_logger test_carbon_logger COMMAND test_carbon_logger )

ADD_EXECUTABLE(simple_example simple_example)
TARGET_LINK_LIBRARIES(simple_example
                      carboncxx )