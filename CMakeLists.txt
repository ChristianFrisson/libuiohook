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
option(BUILD_APPS "Build apps" OFF)

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

if(UNIX AND NOT APPLE)
	message("Linux")
	# x11
	find_package(X11 REQUIRED)

	# xtst
	#find_package(xtst REQUIRED)
	find_library(XTST_LIBRARY NAMES xtst Xtst)
	if(NOT XTST_LIBRARY)
		message(FATAL_ERROR "Could not find xtst")
	endif()
	# XRecordQueryVersion symbol in xtst
	# X11/extensions/record.h

	# option evdev (generic Linux input driver) only for Linux but not UNIX
	OPTION(USE_EVDEV ON "Use Linux input driver")
	if(USE_EVDEV)
		add_definitions(-DUSE_EVDEV)
	endif()

	# option xkb (X Keyboard Extension)
	OPTION(USE_XKB ON "Use X Keyboard Extensions")
	if(USE_XKB)
		if(NOT X11_Xkb_FOUND)
			message(FATAL_ERROR "Could not find X Keyboard Extensions")
		endif()
		add_definitions(-DUSE_XKB)
	endif()

	# option xt (X Toolkit intrisics)
	OPTION(USE_XT "Use X Toolkit intrisics" ON)
	if(USE_XT)
		#find_library(XT_LIBRARY NAMES xt REQUIRED)
		#find_package(xt REQUIRED)
		if(NOT X11_Xt_FOUND)
			message(FATAL_ERROR "Could not find X Toolkit instrisics")
		endif()
		add_definitions(-DUSE_XT)
	endif()

	# option xf86misc (XFree86-Misc X Extension) disabled by default
	OPTION(USE_X86MISC OFF)
	if(USE_X86MISC)
		#find_library(X86MISC_LIBRARY NAMES x86misc REQUIRED)
		#find_package(x86misc REQUIRED)
                if(NOT X11_xf86misc_FOUND)
                        message(FATAL_ERROR "Could not find XFree86-Misc X Extension")
                endif()
		add_definitions(-DUSE_X86MISC)
	endif()

	# option xrecord_async (XRecord Asynchronous API) disabled by default
	OPTION(USE_XRECORD_ASYNC "Use XRecord Asynchronous API" OFF)
	if(USE_XRECORD_ASYNC)
		add_definitions(-DUSE_XRECORD_ASYNC)
		#cflags  -D_POSIX_C_SOURCE_199309L
	endif()

	# option xtest (XTest Extension)
	OPTION(USE_XTEST "Use XTest Extension" ON)
	if(USE_XTEST)
		#check symbol XTestQueryExtension in xtst
		#check headers X11/extensions/XTest.H
                if(NOT X11_XTest_FOUND)
                        message(FATAL_ERROR "Could not find XTest X Extension")
                endif()
		add_definitions(-DUSE_XTEST)
	endif()

	# option xinerama (Xinerama Extension)
	OPTION(USE_XINERAMA "Use Xinerama Extension" ON)

	# option xrandr (XRandR Extension)
	OPTION(USE_XRANDR "Use XRandR Extensions" ON)
	if(USE_XRANDR)
		#find_package(xrandr)
		#find_library(XRANDR_LIBRARY NAMES xrandr REQUIRED)
		#if xrandr not found, find xinerama
		if(NOT X11_Xrandr_FOUND)
			#find_library(XINERAMA_LIBRARY NAMES xinerama REQUIRED)
			if(NOT X11_Xinerama_FOUND)
				message(FATAL_ERROR "Could not find xrandr or xinerama")
			endif()
		endif()
	endif()

	# pthreads required for Async X Record, XRandR and demos
	if(USE_XRANDR OR USE_XRECORD_ASYNC OR BUILD_DEMOS)
		find_package(Threads REQUIRED)
	endif()

	# option xcb (X Protocol C-language binding
	# option xkbcommonn (X Keyboard Common Extension)
	OPTION(USE_XKBCOMMON "Use X Keyboard Common Extension" ON)
	if(USE_XKBCOMMON)
		#find_package(x11_xcb REQUIRED)
		find_library(X11_XCB_LIBRARY NAMES X11-xcb REQUIRED)
		if(NOT X11_XCB_LIBRARY)
			message(FATAL_ERROR "Could not find X11-xcb")
		endif()
		#find_package(xkbcommon REQUIRED)
		find_library(XKBCOMMON_LIBRARY NAMES xkbcommon REQUIRED)
		if(NOT XKBCOMMON_LIBRARY)
                        message(FATAL_ERROR "Could not find xkbcommon")
                endif()
		#find_package(xkbcommon_x11 REQUIRED)
		find_library(XKBCOMMON_X11_LIBRARY NAMES xkbcommon-x11 REQUIRED)
		if(NOT XKBCOMMON_X11_LIBRARY)
                        message(FATAL_ERROR "Could not find xkbcommon-x11")
                endif()
		find_path(XKBCOMMON_X11_XLIB_XCB_INCLUDE_PATH NAMES X11/Xlib-xcb.h REQUIRED)
		if(NOT XKBCOMMON_X11_XLIB_XCB_INCLUDE_PATH)
                        message(FATAL_ERROR "Could not find X11/Xlib-xcb.h")
                endif()
		add_definitions(-DUSE_XKBCOMMON)
	endif()

	# option xkbfile (X Keyboard File Extension)
	OPTION(USE_XKBFILE "X Keyboard File Extensions" ON)
	if(USE_XKBFILE)
		if(NOT USE_XKBCOMMON)
			message(FATAL_ERROR "X Keyboard Common Extensions are required for X Keyboard File Extensions. Either set USE_XKBCOMMON to ON or disable USE_XKBFILE to OFF")
		endif()
		#find_package(xkbfile)
		#find_library(XKBFILE_LIBRARY NAMES xkbfile REQUIRED)
                if(NOT X11_Xkbfile_FOUND)
                        message(FATAL_ERROR "Could not find X Keyboard X Extension")
                endif()
		add_definitions(-DUSE_XKBFILE)
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

if(BUILD_APPS)
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
endif()
