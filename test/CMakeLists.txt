
ADD_EXECUTABLE(test_carbon_metric test_carbon_metric)
TARGET_LINK_LIBRARIES(test_carbon_metric 
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      carboncxx )
ADD_TEST(test_carbon_metric test_carbon_metric COMMAND test_carbon_metric )