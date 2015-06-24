
ADD_LIBRARY(carboncxx SHARED
            carbon_connection carbon_connection_tcp carbon_connection_udp
            carbon_logger carbon_metric )

TARGET_LINK_LIBRARIES( carboncxx
                       pthread ${Boost_LIBRARIES} )
                       
INSTALL(TARGETS carboncxx DESTINATION lib)

INSTALL(FILES carbon_connection_.h carbon_connection_tcp.h carbon_connection_udp.h
              carbon_logger.h carbon_metric.h
              DESTINATION include/carboncxx)
