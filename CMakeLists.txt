# Author: leio
# Date: 2016.11.24
# Desc: 

cmake_minimum_required(VERSION 2.6)
project (NplCefBrowser)

set (BoostRoot_DIR "$ENV{BOOST_ROOT}")
set (CEF3_DIR "${PROJECT_SOURCE_DIR}/deps/cef3")
option(NPLRuntime_DIR "" "../NPLRuntime")
set (NPLRuntime_Absolutely_DIR "${PROJECT_SOURCE_DIR}/${NPLRuntime_DIR}")

set( OUTPUT_BIN_DIR "${NPLRuntime_Absolutely_DIR}/ParaWorld/cef3" )


# offscreen render needs define OSR_RENDER 
#add_definitions(-DOSR_RENDER) 

# cef
add_subdirectory ("${CEF3_DIR}")
set_target_properties(libcef_dll_wrapper
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${CEF3_DIR}
	LIBRARY_OUTPUT_DIRECTORY ${CEF3_DIR}
)
# NplCefPlugin
add_subdirectory ("${PROJECT_SOURCE_DIR}/NplCefPlugin")
set_target_properties(NplCefPlugin
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CEF3_DIR}
	ARCHIVE_OUTPUT_DIRECTORY ${CEF3_DIR}
	LIBRARY_OUTPUT_DIRECTORY ${CEF3_DIR}
)
# NplCefProcess
add_subdirectory ("${PROJECT_SOURCE_DIR}/NplCefProcess")
set_target_properties(NplCefProcess
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CEF3_DIR}
	ARCHIVE_OUTPUT_DIRECTORY ${CEF3_DIR}
	LIBRARY_OUTPUT_DIRECTORY ${CEF3_DIR}
)

# NplCefPlugin
add_subdirectory ("${PROJECT_SOURCE_DIR}/HelloWorldPlugin")
set_target_properties(HelloWorldPlugin
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CEF3_DIR}
	ARCHIVE_OUTPUT_DIRECTORY ${CEF3_DIR}
	LIBRARY_OUTPUT_DIRECTORY ${CEF3_DIR}
)
