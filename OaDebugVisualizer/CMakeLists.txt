cmake_minimum_required (VERSION 3.0.0)
project (OaDebugVisualizer)

set(SOURCES
  OaVisualizer.cpp
  ConfigMapper.cxx
)
  

add_library(OaDebugVisualizer SHARED ${SOURCES})