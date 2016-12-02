# Author: leio
# Date: 2016.11.24
# Desc: 

cmake_minimum_required(VERSION 2.6)
project (NplCefBrowser)

set (BoostRoot_DIR "$ENV{BOOST_ROOT}")
set (CEF3_DIR "${PROJECT_SOURCE_DIR}/deps/cef3")
option(NPLRuntime_DIR "")
set (NPLRuntime_Absolutely_DIR "${PROJECT_SOURCE_DIR}/${NPLRuntime_DIR}")

add_definitions(-D_INT32)
add_definitions(-D_UINT32)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DNOMINMAX)

SET(NplCefBrowser_Source_files "")

file (GLOB src_files ${PROJECT_SOURCE_DIR}/src/*.*)
SOURCE_GROUP("src" FILES ${src_files}) 
list(APPEND NplCefBrowser_Source_files ${src_files})
# cefclient/common
file (GLOB common_files ${PROJECT_SOURCE_DIR}/src/cefclient/common/*.*)
SOURCE_GROUP("src\\cefclient\\common" FILES ${common_files}) 
list(APPEND NplCefBrowser_Source_files ${common_files})

# cefclient/browser
file (GLOB browser_files ${PROJECT_SOURCE_DIR}/src/cefclient/browser/*.*)
SOURCE_GROUP("src\\cefclient\\browser" FILES ${browser_files}) 
list(APPEND NplCefBrowser_Source_files ${browser_files})

# cefclient/renderer
file (GLOB renderer_files ${PROJECT_SOURCE_DIR}/src/cefclient/renderer/*.*)
SOURCE_GROUP("src\\cefclient\\renderer" FILES ${renderer_files}) 
list(APPEND NplCefBrowser_Source_files ${renderer_files})

# cefclient/resources
file (GLOB resources_files ${PROJECT_SOURCE_DIR}/src/cefclient/resources/*.*)
SOURCE_GROUP("src\\cefclient\\resources" FILES ${resources_files}) 
list(APPEND NplCefBrowser_Source_files ${resources_files})

# cefclient/resources/win
file (GLOB resources_win_files ${PROJECT_SOURCE_DIR}/src/cefclient/resources/win/*.*)
SOURCE_GROUP("src\\cefclient\\resources\\win" FILES ${resources_win_files}) 
list(APPEND NplCefBrowser_Source_files ${resources_win_files})

# cefsimple
file (GLOB cefsimple_files ${PROJECT_SOURCE_DIR}/src/cefsimple/*.*)
SOURCE_GROUP("src\\cefsimple" FILES ${cefsimple_files}) 
list(APPEND NplCefBrowser_Source_files ${cefsimple_files})


include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/Core")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/renderer")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/math")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/util")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/IO")
include_directories("${CEF3_DIR}")
include_directories("${BoostRoot_DIR}")

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

# add the executable
add_library(NplCefBrowser SHARED ${NplCefBrowser_Source_files})
target_link_libraries(NplCefBrowser libcef.lib cef_sandbox.lib libcef_dll_wrapper.lib "${CEF_STANDARD_LIBS}" "${CEF_SANDBOX_STANDARD_LIBS}" glu32.lib opengl32.lib)



IF(WIN32)
	ADD_DEFINITIONS(-DWNT)
ENDIF(WIN32)

if(MSVC)
	get_target_property(DEBUG_DLL_PATH NplCefBrowser DEBUG_LOCATION)
	STRING(REGEX REPLACE "^.*[/\\]([^/\\]+).dll$" "\\1" DEBUG_DLL_NAME "${DEBUG_DLL_PATH}" )
	get_target_property(RELEASE_DLL_PATH NplCefBrowser RELEASE_LOCATION)
	STRING(REGEX REPLACE "^.*[/\\]([^/\\]+).dll$" "\\1" RELEASE_DLL_NAME "${RELEASE_DLL_PATH}" )

	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		set( PARAWORLD_BIN_DIR "${NPLRuntime_Absolutely_DIR}/ParaWorld/bin64/" )
	else()
		set( PARAWORLD_BIN_DIR "${NPLRuntime_Absolutely_DIR}/ParaWorld/" )
	endif()


	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	ADD_CUSTOM_COMMAND(
	   TARGET NplCefBrowser
	   POST_BUILD
	   COMMAND ${CMAKE_COMMAND} -E copy ${DEBUG_DLL_PATH} ${PARAWORLD_BIN_DIR}${DEBUG_DLL_NAME}.dll
	   COMMAND ${CMAKE_COMMAND} -E copy ${DEBUG_DLL_PATH} ${PARAWORLD_BIN_DIR}${DEBUG_DLL_NAME}.pdb
	   COMMAND ${CMAKE_COMMAND} -E copy ${CEF3_DIR}/Debug/libcef.dll ${PARAWORLD_BIN_DIR}libcef.dll
	)
 	endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

	 if (CMAKE_BUILD_TYPE STREQUAL "Release")
	ADD_CUSTOM_COMMAND(
	   TARGET NplCefBrowser
	   POST_BUILD
	   COMMAND ${CMAKE_COMMAND} -E copy ${RELEASE_DLL_PATH} ${PARAWORLD_BIN_DIR}${RELEASE_DLL_NAME}.dll
	   COMMAND ${CMAKE_COMMAND} -E copy ${CEF3_DIR}/Release/libcef.dll ${PARAWORLD_BIN_DIR}libcef.dll
	)
 	endif (CMAKE_BUILD_TYPE STREQUAL "Release")
	
endif(MSVC)