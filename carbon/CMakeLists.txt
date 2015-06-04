
ADD_LIBRARY(carboncxx
            carbon_connection carbon_logger carbon_metric )

TARGET_LINK_LIBRARIES( carboncxx
                       pthread ${Boost_LIBRARIES} )
