set(SOURCES
Network_SocketEndpointConverter_Test.cpp
Network_SocketEndpoint_Test.cpp
strOps_ReplaceAll_Test.cpp
strOps_Tokenize_Test.cpp
)

add_library(UnitTest SHARED ${SOURCES})
target_link_libraries(UnitTest Schublade)