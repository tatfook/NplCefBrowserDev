# Author: leio
# Date: 2016.11.24
# Desc: 

cmake_minimum_required(VERSION 2.6)
project (NplCefBrowser)

add_definitions(-D_INT32)
add_definitions(-D_UINT32)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DNOMINMAX)
#add_definitions(-DCEF_USE_SANDBOX)



set (BoostRoot_DIR "$ENV{BOOST_ROOT}")
set (CEF3_DIR "${PROJECT_SOURCE_DIR}/deps/cef3")

if ("$ENV{NPLRUNTIME_ROOT}" STREQUAL "")
      message("NPLRUNTIME_ROOT environment variable not set")
      set(NPLRuntime_DIR "../NPLRuntime" CACHE STRING "")
      set (NPLRuntime_Absolutely_DIR "${PROJECT_SOURCE_DIR}/${NPLRuntime_DIR}")
      set(ParaEngineClient_SOURCE_DIR ${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient)
else()
	set(ParaEngineClient_SOURCE_DIR $ENV{NPLRUNTIME_ROOT}/Client/trunk/ParaEngineClient)
endif()



set( OUTPUT_BIN_DIR "${NPLRuntime_Absolutely_DIR}/ParaWorld/cef3" )

# Libraries required by cef_sandbox.lib.
set(CEF_SANDBOX_STANDARD_LIBS
      dbghelp.lib
      psapi.lib
      version.lib
      winmm.lib
      )

# Standard libraries.
set(CEF_STANDARD_LIBS
	comctl32.lib
	rpcrt4.lib
	shlwapi.lib
	ws2_32.lib
)

# NplCefProcess
add_subdirectory ("${PROJECT_SOURCE_DIR}/NplCefProcess")
# NplCefPlugin
add_subdirectory ("${PROJECT_SOURCE_DIR}/NplCefPlugin")

