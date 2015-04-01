# Locate Apple IOKit
# This module defines
# IOKIT_LIBRARY
# IOKIT_FOUND, if false, do not try to link to gdal 
# IOKIT_INCLUDE_DIR, where to find the headers
#
# $IOKIT_DIR is an environment variable that would
# correspond to the ./configure --prefix=$IOKIT_DIR
#
# Created by Christian Frisson.

#=============================================================================
# Copyright 2015 Christian Frisson.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

IF(APPLE)
  FIND_PATH(IOKIT_INCLUDE_DIR IOKit/IOKitKeys.h)
  FIND_LIBRARY(IOKIT_LIBRARY IOKIT)
ENDIF()


SET(IOKIT_FOUND "NO")
IF(IOKIT_LIBRARY AND IOKIT_INCLUDE_DIR)
  SET(IOKIT_FOUND "YES")
ENDIF()

