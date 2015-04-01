# Locate Apple ApplicationServices
# This module defines
# APPLICATIONSERVICES_LIBRARY
# APPLICATIONSERVICES_FOUND, if false, do not try to link to gdal 
# APPLICATIONSERVICES_INCLUDE_DIR, where to find the headers
#
# $APPLICATIONSERVICES_DIR is an environment variable that would
# correspond to the ./configure --prefix=$APPLICATIONSERVICES_DIR
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
  FIND_PATH(APPLICATIONSERVICES_INCLUDE_DIR ApplicationServices/ApplicationServices.h)
  FIND_LIBRARY(APPLICATIONSERVICES_LIBRARY ApplicationServices)
ENDIF()


SET(APPLICATIONSERVICES_FOUND "NO")
IF(APPLICATIONSERVICES_LIBRARY AND APPLICATIONSERVICES_INCLUDE_DIR)
  SET(APPLICATIONSERVICES_FOUND "YES")
ENDIF()

