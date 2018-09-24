set(SOURCES
Tcp_Test.cpp
)

add_library(IntegrationTest SHARED ${SOURCES})
target_link_libraries(IntegrationTest Schublade)