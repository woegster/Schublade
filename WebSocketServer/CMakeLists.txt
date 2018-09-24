set(SOURCES
WebSocketServer.cpp
)

add_executable(WebSocketServer ${SOURCES})
target_link_libraries(WebSocketServer Schublade)