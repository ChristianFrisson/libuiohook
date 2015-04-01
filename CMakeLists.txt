#=============================================================================
# CMake integration Copyright 2015 Christian Frisson.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

IF(CMAKE_TOOLCHAIN_FILE AND CMAKE_INSTALL_PREFIX)
	SET(CMAKE_FIND_ROOT_PATH "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Installation Directory" FORCE)
	MESSAGE("Using find root path: ${CMAKE_FIND_ROOT_PATH}")
ENDIF()

PROJECT(libuiohook)

# add some find scripts
# SET(CMAKE_USE_RELATIVE_PATHS YES)
#SET(CMAKE_MODULE_PATH "${libuiohook_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATHS "${libuiohook_SOURCE_DIR}/cmake")
FILE(GLOB CMAKEDIRLIST ${CMAKE_SOURCE_DIR}/cmake/*)
FOREACH(CMAKEDIR ${CMAKEDIRLIST})
    IF(IS_DIRECTORY ${CMAKEDIR})
	list(APPEND CMAKE_MODULE_PATHS "${CMAKEDIR}")
    ENDIF()
ENDFOREACH()
SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATHS}")

SET(libuiohook_VERSION 1.0.0)

#MESSAGE("Current libuiohook revision: ${libuiohook_VERSION}")
SET(CMAKE_CXX_FLAGS "-DST_VERSION=${libuiohook_VERSION}")
SET(CMAKE_C_FLAGS "-DST_VERSION=${libuiohook_VERSION}")

cmake_minimum_required(VERSION 2.6.2)
#MESSAGE(${CMAKE_VERSION})
# CMake 2.8.6+ required for setting/getting properties between subprojects
IF("${CMAKE_MAJOR_VERSION}" GREATER "1" AND "${CMAKE_MINOR_VERSION}" GREATER "7" AND "${CMAKE_PATCH_VERSION}" GREATER "5")
    SET(CMAKE_CAN_APPEND_PROPERTIES ON)
ENDIF()

IF(${PROJECT_BINARY_DIR} EQUAL ${PROJECT_SOURCE_DIR})
MESSAGE(FATAL_ERROR "In-source building forbidden: create a 'Builds' subdirectory itself containing a directory describing your system and architecture.")
ENDIF()

IF(COMMAND cmake_policy)
	IF ( ${CMAKE_VERSION} STRLESS "2.8.0")
		#AM: don't know which is the best, but must be < CMP0008 for cmake 2.6.0 and < CMP0012 for cmake 2.6.4
		cmake_policy(SET CMP0003 NEW) 
	ELSE()
		cmake_policy(SET CMP0014 NEW)
                cmake_policy(SET CMP0020 NEW) # Automatically link Qt executables to qtmain target on Windows
                cmake_policy(SET CMP0022 NEW) # CF for INTERFACE_LINK_LIBRARIES defines the link interface.
                cmake_policy(SET CMP0046 NEW) # CF comment this to debug the empty ADD_DEPENDENCIES
		cmake_policy(SET CMP0042 NEW) # otherwise complaints that "MACOSX_RPATH is not specified for the following targets"
	ENDIF ( )
ENDIF(COMMAND cmake_policy)

# Checking Apple OSX versions and generator
IF (APPLE)
	IF(${CMAKE_SYSTEM_VERSION} MATCHES "11.")
		SET(APPLE_LION ON)
		ADD_DEFINITIONS(-DAPPLE_LION)
	ELSEIF(${CMAKE_SYSTEM_VERSION} MATCHES "10.")
		SET(APPLE_SNOW_LEOPARD ON)
		ADD_DEFINITIONS(-DAPPLE_SNOW_LEOPARD)
	ELSEIF(${CMAKE_SYSTEM_VERSION} MATCHES "9.")
		SET(APPLE_LEOPARD ON)
		ADD_DEFINITIONS(-DAPPLE_LEOPARD)
	ENDIF()
	IF("${CMAKE_GENERATOR}" MATCHES "Xcode")
		FIND_PROGRAM(XCODEBUILD NAMES xcodebuild)
		IF(XCODEBUILD)
			EXEC_PROGRAM(${XCODEBUILD} ARGS "-version" OUTPUT_VARIABLE XCODE_VERSION_OUT RESULT_VARIABLE XCODE_VERSION_RES)
			STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\1" XCODE_VERSION_MAJOR ${XCODE_VERSION_OUT})
			STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\2" XCODE_VERSION_MINOR ${XCODE_VERSION_OUT})
			STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\3" XCODE_VERSION_PATCH ${XCODE_VERSION_OUT})
			STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\1.\\2.\\3" XCODE_VERSION ${XCODE_VERSION_OUT})
			IF(${XCODE_VERSION_MAJOR} LESS 4)
				MESSAGE("Using Xcode version ${XCODE_VERSION}, activating XCODE_OLD definition")
				SET(XCODE_OLD ON)
				ADD_DEFINITIONS(-DXCODE_OLD)
			ELSE()
				MESSAGE("Using Xcode version ${XCODE_VERSION}")
			ENDIF()
		ELSE()
			MESSAGE("Using Xcode of unidentified version")
		ENDIF()
		ADD_DEFINITIONS(-DXCODE)
	ENDIF()
ENDIF()

# Determine compiler versions
EXEC_PROGRAM(${CMAKE_C_COMPILER}
	ARGS --version
	OUTPUT_VARIABLE CMAKE_C_COMPILER_VERSION
)
STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\1" CMAKE_C_COMPILER_VERSION_MAJOR ${CMAKE_C_COMPILER_VERSION})
STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\2" CMAKE_C_COMPILER_VERSION_MINOR ${CMAKE_C_COMPILER_VERSION})
STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\3" CMAKE_C_COMPILER_VERSION_PATCH ${CMAKE_C_COMPILER_VERSION})
STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\1.\\2.\\3" CMAKE_C_COMPILER_VERSION ${CMAKE_C_COMPILER_VERSION})
#MESSAGE("CMAKE_C_COMPILER_VERSION_MAJOR: ${CMAKE_C_COMPILER_VERSION_MAJOR}")
#MESSAGE("CMAKE_C_COMPILER_VERSION_MINOR: ${CMAKE_C_COMPILER_VERSION_MINOR}")
#MESSAGE("CMAKE_C_COMPILER_VERSION_PATCH: ${CMAKE_C_COMPILER_VERSION_PATCH}")

#MESSAGE("CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
EXEC_PROGRAM(${CMAKE_CXX_COMPILER}
	ARGS --version
	OUTPUT_VARIABLE CMAKE_CXX_COMPILER_VERSION
)
STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\1" CMAKE_CXX_COMPILER_VERSION_MAJOR ${CMAKE_CXX_COMPILER_VERSION})
STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\2" CMAKE_CXX_COMPILER_VERSION_MINOR ${CMAKE_CXX_COMPILER_VERSION})
STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\3" CMAKE_CXX_COMPILER_VERSION_PATCH ${CMAKE_CXX_COMPILER_VERSION})
STRING(REGEX REPLACE ".*([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\1.\\2.\\3" CMAKE_CXX_COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
#MESSAGE("CMAKE_CXX_COMPILER_VERSION_MAJOR: ${CMAKE_CXX_COMPILER_VERSION_MAJOR}")
#MESSAGE("CMAKE_CXX_COMPILER_VERSION_MINOR: ${CMAKE_CXX_COMPILER_VERSION_MINOR}")
#MESSAGE("CMAKE_CXX_COMPILER_VERSION_PATCH: ${CMAKE_CXX_COMPILER_VERSION_PATCH}")

GET_FILENAME_COMPONENT(COMPILER ${CMAKE_C_COMPILER} NAME_WE)
STRING(COMPARE EQUAL "${COMPILER}" "clang" IS_CLANG) 
IF(0)#NOT IS_CLANG) # check symlinks first: /usr/bin/cc -> clang
IF(CMAKE_C_COMPILER_VERSION_MAJOR LESS 5 AND CMAKE_C_COMPILER_VERSION_MINOR LESS 2 AND CMAKE_CXX_COMPILER_VERSION_MAJOR LESS 5 AND CMAKE_CXX_COMPILER_VERSION_MINOR LESS 2) # we want at least 4.2 for OpenMP and old Xcode versions
	include(CMakeForceCompiler)
	#CMAKE_FORCE_C_COMPILER(gcc-4.2 GNU) #CF
	#CMAKE_FORCE_CXX_COMPILER(gcc-4.2 GNU) #CF
	IF(APPLE AND XCODE)
		set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "4.2")
	ENDIF()
	MESSAGE("Forced C/C++ compiler versions to 4.2")
ENDIF()
ENDIF()

IF(CMAKE_C_COMPILER_VERSION_MAJOR EQUAL 4 AND CMAKE_C_COMPILER_VERSION_MINOR EQUAL 7)
	#http://gcc.gnu.org/gcc-4.7/porting_to.html
	#MESSSAGE("using -fpermissive")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive") # for templates in armadillo
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpermissive")
ENDIF()

IF(APPLE_SNOW_LEOPARD)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99") # for templates in armadillo
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
ENDIF()

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)

######################
#       OPTIONS      #
######################

OPTION(BUILD_SHARED_LIBS "Build libuiohook libraries as shared" OFF)
OPTION(USE_DEBUG "Use Debug" ON)
OPTION(BUILD_DOCUMENTATION "Build libuiohook reference documentation using doxygen (use: make documentation)" OFF)

# Current working config for cross-compiling with MXE
IF(MINGW AND CMAKE_TOOLCHAIN_FILE)
	SET(BUILD_SHARED_LIBS OFF)
	SET(ST_PLUGINS_STATIC ON)
ENDIF()

######################
#     OS SPECIFIC    #
######################

# uiohook Apple requirements: -framework ApplicationServices -framework IOKit -framework Carbon

IF(APPLE)
	FIND_PACKAGE(ApplicationServices)
	IF(APPLICATIONSERVICES_FOUND)
		MESSAGE("ApplicationServices found: ${APPLICATIONSERVICES_LIBRARY}")
	ELSE()
		MESSAGE(FATAL_ERROR "ApplicationServices not found")
	ENDIF()
	FIND_PACKAGE(Cocoa)
	IF(COCOA_FOUND)
		MESSAGE("Cocoa found: ${COCOA_LIBRARY}")
	ELSE()
		MESSAGE(FATAL_ERROR "Foundation not found")
	ENDIF()
	FIND_PACKAGE(IOKit)
	IF(IOKIT_FOUND)
		MESSAGE("IOKit found: ${IOKIT_LIBRARY}")
	ELSE()
		MESSAGE(FATAL_ERROR "IOKit not found")
	ENDIF()
ENDIF()

# OS-SPECIFIC SUBPROJECTS COMPILATION
IF(UNIX)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)

# Ubuntu detection
IF(UNIX)
	# Try to find architecture
	execute_process(COMMAND uname -m OUTPUT_VARIABLE CPACK_PACKAGE_ARCHITECTURE)
	string(STRIP "${CPACK_PACKAGE_ARCHITECTURE}" CPACK_PACKAGE_ARCHITECTURE)
	# Try to find distro name and distro-specific arch
	execute_process(COMMAND lsb_release -is OUTPUT_VARIABLE LSB_ID)
	execute_process(COMMAND lsb_release -rs OUTPUT_VARIABLE LSB_RELEASE)
	string(STRIP "${LSB_ID}" LSB_ID)
	string(STRIP "${LSB_RELEASE}" LSB_RELEASE)
	set(LSB_DISTRIB "${LSB_ID}${LSB_RELEASE}")
	IF(NOT LSB_DISTRIB)
		set(LSB_DISTRIB "unix")
	ENDIF(NOT LSB_DISTRIB)
ENDIF()

# Apple architectures
IF(APPLE AND NOT APPLE_IOS_XCODE AND NOT APPLE_IOS_SIM AND NOT APPLE_IOS_XCODE)
		IF(${CMAKE_SYSTEM_VERSION} GREATER "11.") # Mac OS X 10.7 and above
			SET(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Building for x86_64" FORCE)
		ELSE()
			SET(CMAKE_OSX_ARCHITECTURES "i386;x86_64" CACHE STRING "Building for i386 (FFmpeg and QTKit support currently broken on Snow Leopard)" FORCE)
		ENDIF()
ENDIF()

# Win32
IF(MINGW AND CMAKE_TOOLCHAIN_FILE)
	MESSAGE("Cross-compiling for windows with mingw")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	SET(BUILD_SHARED_LIBS OFF)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
ENDIF()

IF(APPLE) # assuming all are dynamic libraries for now
    SET(PLUGIN_SUFFIX "dylib") 
ELSE()
    IF(UNIX)
        SET(PLUGIN_SUFFIX "so") # since CMAKE_SHARED_LIBRARY_PREFIX returns "lib" on Ubuntu
    ELSE()
        SET(PLUGIN_SUFFIX "dll")
    ENDIF()
ENDIF()

IF ( WIN32 )
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_DEPRECATE")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_DEPRECATE")
	SET(BUILD_SHARED_LIBS OFF)
ENDIF ( WIN32 )

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
# EXTERNAL DEPENDENCIES #
#########################

MESSAGE("\nExternal libraries:")

#########################
# INTERNAL DEPENDENCIES #
#########################

MESSAGE("\nInternal dependencies:\n")

# Core dependencies
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(src)

SET(ARCHDIR "")
IF(APPLE)
	SET(ARCHDIR "darwin")
ELSEIF(WIN32 OR WIN64)
	SET(ARCHDIR "windows")
ELSEIF(UNIX)
	SET(ARCHDIR "x11")
ELSE()
	MESSAGE(FATAL_ERROR "Architecture not supported")
ENDIF()

INCLUDE_DIRECTORIES(src/${ARCHDIR})

######################
# INTERNAL LIBRARIES #
######################
MESSAGE("\nInternal libraries:")

FILE(GLOB LIBUIOHOOK_SRC src/logger.c src/${ARCHDIR}/*.c)
FILE(GLOB LIBUIOHOOK_HDR src/*.h src/${ARCHDIR}/*.h)

ADD_LIBRARY(uiohook ${LIBUIOHOOK_SRC} ${LIBUIOHOOK_HDR})

IF(APPLE)
	TARGET_LINK_LIBRARIES(uiohook  ${APPLICATIONSERVICES_LIBRARY} ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
ENDIF()

######################
#         APPS       #
######################

#add_subdirectory()
FILE(GLOB LIBUIOHOOK_DEMO_SRCS src/demo*.c)
FOREACH(LIBUIOHOOK_DEMO_SRC ${LIBUIOHOOK_DEMO_SRCS})
	GET_FILENAME_COMPONENT(LIBUIOHOOK_DEMO ${LIBUIOHOOK_DEMO_SRC} NAME_WE)
	#MESSAGE("uiohook demo src ${LIBUIOHOOK_DEMO}")
	ADD_EXECUTABLE(${LIBUIOHOOK_DEMO} ${LIBUIOHOOK_DEMO_SRC})
	TARGET_LINK_LIBRARIES(${LIBUIOHOOK_DEMO} uiohook)
ENDFOREACH()

######################
#        TESTS       #
######################

#add_subdirectory(test) #TODO
