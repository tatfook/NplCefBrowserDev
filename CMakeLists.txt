# Author: leio
# Date: 2016.11.24
# Desc: 

cmake_minimum_required(VERSION 2.6)
project (NplCefBrowser)

set (BoostRoot_DIR "$ENV{BOOST_ROOT}")
set (CEF3_DIR "${PROJECT_SOURCE_DIR}/deps/cef3")
option(NPLRuntime_DIR "")

add_definitions(-D_INT32)
add_definitions(-D_UINT32)

file (GLOB Source_files RELATIVE ${PROJECT_SOURCE_DIR}
    src/*.cpp
	src/*.h
    )
SOURCE_GROUP("src" FILES ${Source_files}) 

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${NPLRuntime_DIR}/Client/trunk/ParaEngineClient")
include_directories("${NPLRuntime_DIR}/Client/trunk/ParaEngineClient/Core")
include_directories("${NPLRuntime_DIR}/Client/trunk/ParaEngineClient/renderer")
include_directories("${NPLRuntime_DIR}/Client/trunk/ParaEngineClient/math")
include_directories("${NPLRuntime_DIR}/Client/trunk/ParaEngineClient/util")
include_directories("${NPLRuntime_DIR}/Client/trunk/ParaEngineClient/IO")
include_directories("${CEF3_DIR}")
include_directories("${BoostRoot_DIR}")

include_directories("D:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include")
IF(MSVC)
	# statically link MSVC to reduce dependancies
	foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
	 	if(${flag_var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
	 	endif(${flag_var} MATCHES "/MD")
	 	if(${flag_var} MATCHES "/MDd")
	 		string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
		endif(${flag_var} MATCHES "/MDd")
	endforeach(flag_var)
	
	# /GR- Remove RTTI to miminize the executable size
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GR-")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
ENDIF()


IF(MSVC)
	# generate program database (PDB symbol files even in release build)
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
ENDIF(MSVC)

SET(CMAKE_DEBUG_POSTFIX "_d")

link_directories(${PROJECT_SOURCE_DIR}/deps/cef3/${CMAKE_BUILD_TYPE})


# add the executable
add_library(NplCefBrowser SHARED ${Source_files})
target_link_libraries(NplCefBrowser libcef.lib cef_sandbox.lib libcef_dll_wrapper.lib)



IF(WIN32)
	ADD_DEFINITIONS(-DWNT)
ENDIF(WIN32)

if(MSVC)
	get_target_property(DEBUG_DLL_PATH NplCefBrowser DEBUG_LOCATION)
	STRING(REGEX REPLACE "^.*[/\\]([^/\\]+).dll$" "\\1" DEBUG_DLL_NAME "${DEBUG_DLL_PATH}" )
     message("DEBUG_DLL_NAME")
     message(${DEBUG_DLL_NAME})

	get_target_property(RELEASE_DLL_PATH NplCefBrowser RELEASE_LOCATION)
	STRING(REGEX REPLACE "^.*[/\\]([^/\\]+).dll$" "\\1" RELEASE_DLL_NAME "${RELEASE_DLL_PATH}" )

	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		set( PARAWORLD_BIN_DIR "${PROJECT_SOURCE_DIR}/${NPLRuntime_DIR}/ParaWorld/bin64/" )
	else()
		set( PARAWORLD_BIN_DIR "${PROJECT_SOURCE_DIR}/${NPLRuntime_DIR}/ParaWorld/" )
	endif()


	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
     message("debug mode")
	ADD_CUSTOM_COMMAND(
	   TARGET NplCefBrowser
	   POST_BUILD
	   COMMAND ${CMAKE_COMMAND} -E copy ${DEBUG_DLL_PATH} ${PARAWORLD_BIN_DIR}${DEBUG_DLL_NAME}.dll
	   COMMAND ${CMAKE_COMMAND} -E copy ${DEBUG_DLL_PATH} ${PARAWORLD_BIN_DIR}${DEBUG_DLL_NAME}.pdb
	   COMMAND ${CMAKE_COMMAND} -E copy ${CEF3_DIR}/Debug/libcef.dll ${PARAWORLD_BIN_DIR}libcef.dll
	)
 	endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

	 if (CMAKE_BUILD_TYPE STREQUAL "Release")
     message("release mode")
	ADD_CUSTOM_COMMAND(
	   TARGET NplCefBrowser
	   POST_BUILD
	   COMMAND ${CMAKE_COMMAND} -E copy ${RELEASE_DLL_PATH} ${PARAWORLD_BIN_DIR}${RELEASE_DLL_NAME}.dll
	   COMMAND ${CMAKE_COMMAND} -E copy ${CEF3_DIR}/Release/libcef.dll ${PARAWORLD_BIN_DIR}libcef.dll
	)
 	endif (CMAKE_BUILD_TYPE STREQUAL "Release")
	
endif(MSVC)