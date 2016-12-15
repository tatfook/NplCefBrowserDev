# Author: leio
# Date: 2016.12.7
# Desc: 

cmake_minimum_required(VERSION 2.6)
project (HelloWorldPlugin)


SET(NplCefPlugin_Source_files "")
file (GLOB src_files ${PROJECT_SOURCE_DIR}/*.*)
SOURCE_GROUP("src" FILES ${src_files}) 
list(APPEND NplCefPlugin_Source_files ${src_files})



include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/Core")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/renderer")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/math")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/util")
include_directories("${NPLRuntime_Absolutely_DIR}/Client/trunk/ParaEngineClient/IO")
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

link_directories(${CEF3_DIR}/${CMAKE_BUILD_TYPE})



# add the executable
add_library(HelloWorldPlugin SHARED ${NplCefPlugin_Source_files})
target_link_libraries(HelloWorldPlugin libcef.lib libcef_dll_wrapper.lib "${CEF_STANDARD_LIBS}" "${CEF_SANDBOX_STANDARD_LIBS}")

file(GLOB  CEF3_ALL_BINARY_FILES "${CEF3_DIR}/${CMAKE_BUILD_TYPE}/*.dll" "${CEF3_DIR}/${CMAKE_BUILD_TYPE}/*.exe" "${CEF3_DIR}/${CMAKE_BUILD_TYPE}/*.bin")
list(APPEND CEF3_ALL_FILES ${CEF3_ALL_BINARY_FILES})

file(GLOB  CEF3_ALL_RESOURCES_FILES "${CEF3_DIR}/Resources/*.*" )
list(APPEND CEF3_ALL_FILES ${CEF3_ALL_RESOURCES_FILES})

file(GLOB  CEF3_ALL_RESOURCES_LOCALES_FILES "${CEF3_DIR}/Resources/locales/*.*" )

ADD_CUSTOM_COMMAND(
   TARGET HelloWorldPlugin
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_BIN_DIR}
   #COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_BIN_DIR}/locales
   COMMAND ${CMAKE_COMMAND} -E copy ${CEF3_ALL_FILES} ${OUTPUT_BIN_DIR}
   #COMMAND ${CMAKE_COMMAND} -E copy ${CEF3_ALL_RESOURCES_LOCALES_FILES} ${OUTPUT_BIN_DIR}/locales
)