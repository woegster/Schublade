cmake_minimum_required(VERSION 3.1)
project(Win32App)

SET(SOURCES
Win32App.cpp
)

add_executable(Win32App WIN32 ${SOURCES})
target_compile_definitions(
Win32App
 PRIVATE
   NOMINMAX
   NOMCX
   STRICT
   WIN32_LEAN_AND_MEAN
   UNICODE
)