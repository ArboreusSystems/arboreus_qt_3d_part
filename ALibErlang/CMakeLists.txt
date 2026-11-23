cmake_minimum_required(VERSION 3.16)


## ----------------------------------------
## Global settings

set(A_NAME_TARGET ALibErlang)
set(A_PREFIX A_${A_NAME_TARGET})
set(${A_PREFIX}_DIR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})


## ----------------------------------------
## Global configuration

set(${A_PREFIX}_GLOBAL_CONF "${${A_PREFIX}_DIR_ROOT}/../3dPart.conf.cmake")
if(EXISTS ${${A_PREFIX}_GLOBAL_CONF})
	include(${${A_PREFIX}_GLOBAL_CONF})
	message(STATUS "Added global configuration file: " ${${A_PREFIX}_GLOBAL_CONF})
else()
	message(FATAL_ERROR "No global configuration file: " ${${A_PREFIX}_GLOBAL_CONF})
endif()


## ----------------------------------------
## Project settings

project(${A_NAME_TARGET} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(A_OPTION_USING_PCH "${A_NAME_TARGET} using PCH support" ON)


## ----------------------------------------
## Library variables

set(${A_PREFIX}_SOURCES)
set(${A_PREFIX}_SOURCES_PRECOMPILED_HEADERS_PUBLIC)
set(${A_PREFIX}_SOURCES_PRECOMPILED_HEADERS_PRIVATE)
set(${A_PREFIX}_SOURCES_HEADERS)
set(${A_PREFIX}_SOURCES_CPP)
set(${A_PREFIX}_SOURCES_RESOURCES)

set(${A_PREFIX}_INCLUDE_DIR_PUBLIC)
set(${A_PREFIX}_INCLUDE_DIR_PRIVATE)

set(${PREFIX}_LIB_ERLANG_INCLUDE)
set(${PREFIX}_LIB_ERLANG)


## ----------------------------------------
## Library sources

set(${A_PREFIX}_SOURCES_PRECOMPILED_HEADERS_PUBLIC
	${${A_PREFIX}_SOURCES_PRECOMPILED_HEADERS_PUBLIC}
	aliberlang_pch.h
)

set(${A_PREFIX}_SOURCES_HEADERS
	${${A_PREFIX}_SOURCES_HEADERS}
	aliberlang.h
	aliberlang_private.h
)

set(${A_PREFIX}_SOURCES_CPP
	${${A_PREFIX}_SOURCES_CPP}
	aliberlang.cpp
)

set(${A_PREFIX}_SOURCES
	${${A_PREFIX}_SOURCES}
	${${A_PREFIX}_SOURCES_PRECOMPILED_HEADERS_PRIVATE}
	${${A_PREFIX}_SOURCES_PRECOMPILED_HEADERS_PUBLIC}
	${${A_PREFIX}_SOURCES_HEADERS}
	${${A_PREFIX}_SOURCES_CPP}
	${${A_PREFIX}_SOURCES_RESOURCES}
)

set(${A_PREFIX}_INCLUDE_DIR_PUBLIC
	${${A_PREFIX}_INCLUDE_DIR_PUBLIC}
	${${A_PREFIX}_DIR_ROOT}
)

if(APPLE)

	set(${A_PREFIX}_LIB_ERLANG_INCLUDE ${A_GLOBAL_DIR_LIB_ERLANG}/macos/include)
	set(${A_PREFIX}_LIB_ERLANG ${A_GLOBAL_DIR_LIB_ERLANG}/macos/lib/liberlang.a)

endif()

if(IOS)

	set(${A_PREFIX}_IOS_TYPE "ios")
	if(${CMAKE_OSX_SYSROOT} MATCHES "iphonesimulator")
		set(${A_PREFIX}_IOS_TYPE "ios-simulator")
	endif()

	set(${A_PREFIX}_LIB_ERLANG_INCLUDE ${A_GLOBAL_DIR_LIB_ERLANG}/${${A_PREFIX}_IOS_TYPE}/include)
	set(${A_PREFIX}_LIB_ERLANG ${A_GLOBAL_DIR_LIB_ERLANG}/${${A_PREFIX}_IOS_TYPE}/lib/liberlang.a)

endif()

if(ANDROID)

	set(${A_PREFIX}_ANDROID_ARCH ${ANDROID_ABI})
	set(${A_PREFIX}_LIB_ERLANG_INCLUDE ${A_GLOBAL_DIR_LIB_ERLANG}/android/${${A_PREFIX}_ANDROID_ARCH}/include)
	set(${A_PREFIX}_LIB_ERLANG ${A_GLOBAL_DIR_LIB_ERLANG}/android/${${A_PREFIX}_ANDROID_ARCH}/lib/liberlang.a)

endif()


## ----------------------------------------
## Platforms settings

if(APPLE)

endif()

if(IOS)

endif()

if(ANDROID)

	set(QT_FORCE_CMP0156_TO_VALUE NEW)

endif()


## ----------------------------------------
## Qt settings

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.8)
qt_add_library(${A_NAME_TARGET} STATIC ${${A_PREFIX}_SOURCES})

target_link_libraries(${A_NAME_TARGET}
	PRIVATE
		Qt6::Core
		ALogger
		AOpenSSL
)

target_compile_definitions(${A_NAME_TARGET}
	PRIVATE
		ALIBERLANG_LIBRARY
)

target_include_directories(${A_NAME_TARGET}
	PUBLIC
		${${A_PREFIX}_INCLUDE_DIR_PUBLIC}
		${${A_PREFIX}_LIB_ERLANG_INCLUDE}
	PRIVATE
		${${A_PREFIX}_INCLUDE_DIR_PRIVATE}
)


## ----------------------------------------
## Precompiled headers settings

if(
	NOT ${${A_PREFIX}_SOURCES_PRECOMPILED_HEADERS_PRIVATE} EQUAL "" OR
	NOT ${${A_PREFIX}_SOURCES_PRECOMPILED_HEADERS_PUBLIC} EQUAL ""
)
	if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.16 AND A_OPTION_USING_PCH)
		message(STATUS "Using PCH for ${A_NAME_TARGET}")
		target_precompile_headers(${A_NAME_TARGET}
			PRIVATE ${${A_PREFIX}_SOURCES_PRECOMPILED_HEADERS_PRIVATE}
			PUBLIC ${${A_PREFIX}_SOURCES_PRECOMPILED_HEADERS_PUBLIC}
		)
		set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)
	else()
		message(STATUS "No PCH for ${A_NAME_TARGET}")
	endif()
endif()
