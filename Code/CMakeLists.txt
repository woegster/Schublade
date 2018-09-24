set(SOURCES
CpuUsageSampler.cpp
CpuUsageSampler.h
DllInjector.cpp
DllInjector.h
networkCommon.h
ProcUtils.cpp
ProcUtils.h
SocketEndpoint.cpp
SocketEndpoint.h
SocketEndpointConverter.cpp
SocketEndpointConverter.h
strOps.h
TcpClient.cpp
TcpClient.h
TcpServer.cpp
TcpServer.h
Sha1.cpp
Sha1.h
Base64.cpp
Base64.h
)

add_library(Schublade STATIC ${SOURCES})

target_link_libraries(Schublade
  INTERFACE
    ws2_32.lib
)

target_compile_definitions(Schublade
  PUBLIC
    UNICODE
    _UNICODE
    STRICT
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

target_include_directories(Schublade
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)