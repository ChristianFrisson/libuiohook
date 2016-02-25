#=============================================================================
# CMake integration Copyright 2015-present Christian Frisson.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

cmake_minimum_required(VERSION 2.8.12)

project(libuiohook)

######################
#       OPTIONS      #
######################

option(BUILD_SHARED_LIBS "Build libuiohook libraries as shared" OFF)
option(USE_DEBUG "Use Debug" ON)

######################
#     OS SPECIFIC    #
######################

# uiohook Apple requirements: -framework ApplicationServices -framework IOKit -framework Carbon

if(APPLE)
	find_library(APPLICATIONSERVICES_LIBRARY ApplicationServices)
	if(APPLICATIONSERVICES_LIBRARY)
		message(STATUS "Found ApplicationServices: ${APPLICATIONSERVICES_LIBRARY}")
	else()
		message(FATAL_ERROR "ApplicationServices not found")
	endif()
	find_library(COCOA_LIBRARY Cocoa)
	if(COCOA_LIBRARY)
		message(STATUS "Found Cocoa: ${COCOA_LIBRARY}")
	else()
		message(FATAL_ERROR "Foundation not found")
	endif()
	find_library(IOKIT_LIBRARY IOKit)
	if(IOKIT_LIBRARY)
		message(STATUS "Found IOKit: ${IOKIT_LIBRARY}")
	else()
		message(FATAL_ERROR "IOKit not found")
	endif()
endif()

IF ( USE_DEBUG )
	MESSAGE( "\nConfigured for Debug Build")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_DEBUG -DTI_DEBUG=1")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_DEBUG -DTI_DEBUG=1")
        IF(NOT WIN32)
            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
        ENDIF()
	SET(CMAKE_BUILD_TYPE "Debug")
	SET(CMAKE_VERBOSE_MAKEFILE ON)
ELSE ( USE_DEBUG )
	MESSAGE( "\nConfigured for Release Build")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTI_DEBUG=0 -O3")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTI_DEBUG=0 -O3")
	SET(CMAKE_BUILD_TYPE "MinSizeRel")#Debug|Release|RelWithDebInfo|MinSizeRel
	SET(CMAKE_VERBOSE_MAKEFILE ON)#CF
	IF(APPLE)
		SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/release")
	ENDIF()
ENDIF ( USE_DEBUG )

#########################
# INTERNAL DEPENDENCIES #
#########################

# Core dependencies
include_directories(include)
include_directories(src)

set(ARCHDIR "")
if(APPLE)
	set(ARCHDIR "darwin")
elseif(WIN32 OR WIN64)
	set(ARCHDIR "windows")
elseif(UNIX)
	set(ARCHDIR "x11")
else()
	message(FATAL_ERROR "Architecture not supported")
endif()

include_directories(src/${ARCHDIR})

######################
# INTERNAL LIBRARIES #
######################

file(GLOB LIBUIOHOOK_SRC src/logger.c src/${ARCHDIR}/*.c)
file(GLOB LIBUIOHOOK_HDR src/*.h src/${ARCHDIR}/*.h)

add_library(uiohook ${LIBUIOHOOK_SRC} ${LIBUIOHOOK_HDR})

if(APPLE)
	target_link_libraries(uiohook ${APPLICATIONSERVICES_LIBRARY} ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
endif()

######################
#         APPS       #
######################

file(GLOB LIBUIOHOOK_DEMO_SRCS src/demo*.c)
foreach(LIBUIOHOOK_DEMO_SRC ${LIBUIOHOOK_DEMO_SRCS})
	get_filename_component(LIBUIOHOOK_DEMO ${LIBUIOHOOK_DEMO_SRC} NAME_WE)
	#MESSAGE("uiohook demo src ${LIBUIOHOOK_DEMO}")
	add_executable(${LIBUIOHOOK_DEMO} ${LIBUIOHOOK_DEMO_SRC})
	target_link_libraries(${LIBUIOHOOK_DEMO} uiohook)
endforeach()

file(GLOB LIBUIOHOOK_APP_SRCS src/app*.c)
foreach(LIBUIOHOOK_APP_SRC ${LIBUIOHOOK_APP_SRCS})
	get_filename_component(LIBUIOHOOK_APP ${LIBUIOHOOK_APP_SRC} NAME_WE)
	#MESSAGE("uiohook app src ${LIBUIOHOOK_APP}")
	add_executable(${LIBUIOHOOK_APP} ${LIBUIOHOOK_APP_SRC})
	target_link_libraries(${LIBUIOHOOK_APP} uiohook)
endforeach()

