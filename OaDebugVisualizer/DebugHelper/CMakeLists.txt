cmake_minimum_required(VERSION 3.2.0)
project(DebugHelper)

if ( MSVC )

  set(CMAKE_CXX_FLAGS_DEBUG_INIT          "/MD /Zi /Od /RTC1 /W3")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "/MD /Zi /Ox /W3 /d2Zi+")
  set(CMAKE_CXX_FLAGS_MSCOVERAGE_INIT "/MD /Zi /Ob0 /W3 /d2Zi+")

  set(CMAKE_C_FLAGS_DEBUG_INIT          "/MD /Zi /Od /RTC1 /W3")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MD /Zi /Ox /W3")
  set(CMAKE_C_FLAGS_MSCOVERAGE_INIT "/MD /Zi /Ob0 /W3")
endif()

if ( "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" )
  set(CMAKE_C_FLAGS_DEBUG_INIT          "-g")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O3 -g -DNDEBUG")
endif()

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  set(CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O3 -g -DNDEBUG")
endif()

if ( "${CMAKE_C_COMPILER_ID}" STREQUAL "SunPro" )
  set(CMAKE_C_FLAGS_DEBUG_INIT          "-g")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-xO4 -g")
endif()

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "SunPro" )
  set(CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-xO4 -g")
endif()

MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE 
    CMAKE_MODULE_LINKER_FLAGS_COVERAGE 
    COMPILE_DEFINITIONS_COVERAGE
)

set(WCCOA_FD_SETSIZE 8192)

add_definitions(-DPVI_VERSION_X=${WCCOA_VERSION_MAJOR}
                -DPVI_VERSION_Y=${WCCOA_VERSION_MINOR}
                -DPVI_VERSION_Z=0
                -DPVI_PATCH_NUM=0
                -DPVSS_VERSION="${WCCOA_VERSION_MAJOR}.${WCCOA_VERSION_MINOR}"
                -DPVSS_VERSION_DISP="${WCCOA_VERSION_DISP}"
                -DPVSS_PATCH="${WCCOA_VERSION_SP}"
                -DPVSS_VERS=${WCCOA_VERS}
                -DPVSS_VERS_DLL="${WCCOA_VERS}"
                -DPVSS_FD_SETSIZE=${WCCOA_FD_SETSIZE}
                -DPVSS_PLATFORM="${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}"
                -DWCCOA_PLATFORM_SUBDIR="${WCCOA_PLATFORM_SUBDIR}"
                -DUSE_OWN_ALLOCATOR
                -DQT_NO_DEBUG
                -DUSE_QT
)

add_definitions(-DLIBS_AS_DLL -DIS_MSWIN__ -DWIN32 -D_BOOL_T_DEFINED -D_SBCS
      -D_CRT_NON_CONFORMING_SWPRINTFS -D_CRT_SECURE_NO_DEPRECATE
      -D_CRT_NONSTDC_NO_DEPRECATE -DNODEF__RPC_H__ -DWINVER=0x0601)

set(WCCOA_API_ROOT "/opt/WinCC_OA/3.16/api" CACHE FILEPATH "full path including api folder of WinCC installation")

set(WCCOA_API_INCLUDE ${WCCOA_API_ROOT}/include)
set(WCCOA_API_BASICS_INCL .
    ${WCCOA_API_INCLUDE}/BCMNew
    ${WCCOA_API_INCLUDE}/Basics/Variables
    ${WCCOA_API_INCLUDE}/Basics/Utilities
    ${WCCOA_API_INCLUDE}/Basics/NoPosix
    ${WCCOA_API_INCLUDE}/Basics/DpBasics
    ${WCCOA_API_INCLUDE}/PVSSUtil
)

set(WCCOA_API_CONFIGS_INCL
    ${WCCOA_API_BASICS_INCL}
    ${WCCOA_API_INCLUDE}/Configs
    ${WCCOA_API_INCLUDE}/Configs/DrvConfigs/ConvSmooth
    ${WCCOA_API_INCLUDE}/Configs/DrvConfigs/ConvSmooth
    ${WCCOA_API_INCLUDE}/Configs/DrvConfigs/DrvCommon
    ${WCCOA_API_INCLUDE}/Configs/EvConfigs/AlertConfs
    ${WCCOA_API_INCLUDE}/Configs/EvConfigs/SimpleEvConfs
    ${WCCOA_API_INCLUDE}/Configs/DBConfigs
)

set(WCCOA_API_DATAPOINT_INCL
    ${WCCOA_API_CONFIGS_INCL}
    ${WCCOA_API_INCLUDE}/Datapoint)

set(WCCOA_API_MESSAGES_INCL
    ${WCCOA_API_DATAPOINT_INCL}
    ${WCCOA_API_INCLUDE}/Messages
    ${BZIP2_INCL})

set(WCCOA_API_MANAGER_INCL
    ${WCCOA_API_MESSAGES_INCL}
    ${WCCOA_API_INCLUDE}/Manager)
    
set(WCCOA_API_V24_INCL
    ${WCCOA_API_INCLUDE}/V24)

set(WCCOA_API_DRIVER_INCL
    ${WCCOA_API_MANAGER_INCL}
    ${WCCOA_API_V24_INCL}
    ${WCCOA_API_INCLUDE}/ComDrv
)
set(WCCOA_API_CTRL_INCL
    ${WCCOA_API_MANAGER_INCL}
    ${WCCOA_API_INCLUDE}/Ctrl)

## LIBS

function(importLibFromApi libName)
  if ( WIN32 )
    set(WCCOA_API_LIBDIR ${WCCOA_API_ROOT}/lib.winnt)
    set(LIBSUFFIX .lib)
    set(LIBTYPE STATIC)
    
    #libbcm is just called bcm on windows
    if ( libName STREQUAL "bcm")
      set(LIBPREFIX )
    else()
      set(LIBPREFIX lib)
    endif()
  else ()
    set(WCCOA_API_LIBDIR ${WCCOA_API_ROOT}/lib.linux)
    set(LIBSUFFIX V316.so)
    set(LIBPREFIX lib)
    set(LIBTYPE SHARED)
  endif()
  
  add_library(WCCOA_API_${libName} ${LIBTYPE} IMPORTED GLOBAL)
  set_target_properties(WCCOA_API_${libName} PROPERTIES IMPORTED_LOCATION
         ${WCCOA_API_LIBDIR}/${LIBPREFIX}${libName}${LIBSUFFIX})
  
  message(STATUS "added WCCOA_API_${libName} as ${WCCOA_API_LIBDIR}/${LIBPREFIX}${libName}${LIBSUFFIX}")
endfunction()

importLibFromApi(bcm)
importLibFromApi(Basics)
importLibFromApi(Configs)
importLibFromApi(Ctrl)
importLibFromApi(Datapoint)
importLibFromApi(Manager)
importLibFromApi(Messages)

set (SOURCES
  DebugHelper.cxx
)

include_directories(${WCCOA_API_MANAGER_INCL})
add_executable(DebugHelperManager ${SOURCES})
target_link_libraries(DebugHelperManager
WCCOA_API_bcm
WCCOA_API_Basics
WCCOA_API_Configs
WCCOA_API_Ctrl
WCCOA_API_Datapoint
WCCOA_API_Manager
WCCOA_API_Messages
)