# Author: leio
# Date: 2016.12.7
# Desc: 

cmake_minimum_required(VERSION 2.6)

project (NplCefProcess)

add_definitions(-D_INT32)
add_definitions(-D_UINT32)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DNOMINMAX)
add_definitions(-DCEF_USE_SANDBOX)

SET(NplCefProcess_Source_files "")

file (GLOB src_files ${PROJECT_SOURCE_DIR}/src/*.*)
SOURCE_GROUP("src" FILES ${src_files}) 
list(APPEND NplCefProcess_Source_files ${src_files})

include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/Core")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${CEF3_DIR}")

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

link_directories(${CEF3_DIR}/${CMAKE_BUILD_TYPE})

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
add_executable(NplCefProcess ${NplCefProcess_Source_files})
set_target_properties(NplCefProcess PROPERTIES OUTPUT_NAME "NplCefProcess")
set_target_properties(NplCefProcess PROPERTIES DEBUG_OUTPUT_NAME "NplCefProcess_d")
target_link_libraries(NplCefProcess libcef.lib cef_sandbox.lib libcef_dll_wrapper.lib "${CEF_STANDARD_LIBS}" "${CEF_SANDBOX_STANDARD_LIBS}" glu32.lib opengl32.lib)


