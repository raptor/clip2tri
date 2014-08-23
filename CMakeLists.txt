# Copyright (c) 2014 Clifford Yapp
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following
# disclaimer in the documentation and/or other materials provided
# with the distribution.
#
# 3. The name of the author may not be used to endorse or promote
# products derived from this software without specific prior written
# permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Minimum required version of CMake
cmake_minimum_required(VERSION 2.8.8)
if(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0007 OLD)
  if(${CMAKE_VERSION} VERSION_GREATER 2.8.3)
    CMAKE_POLICY(SET CMP0017 OLD)
  endif(${CMAKE_VERSION} VERSION_GREATER 2.8.3)
  if ("${CMAKE_VERSION}" VERSION_GREATER 2.99)
    CMAKE_POLICY(SET CMP0026 OLD)
  endif ("${CMAKE_VERSION}" VERSION_GREATER 2.99)
endif(COMMAND CMAKE_POLICY)

# Set CMake project name
project(CLIP2TRI)

#---------------------------------------------------------------------
# CMake derives much of its functionality from modules, often times
# supplied by the proejct itself - CMAKE_MODULE_PATH tells CMake where
# to look for them.  If we are a subbuild, let the parent's
# CMAKE_MODULE_PATH supply files first.
set(CLIP2TRI_CMAKE_DIR "${CLIP2TRI_SOURCE_DIR}/CMake")
if(CLIP2TRI_IS_SUBBUILD)
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CLIP2TRI_CMAKE_DIR}")
else(CLIP2TRI_IS_SUBBUILD)
  set(CMAKE_MODULE_PATH "${CLIP2TRI_CMAKE_DIR};${CMAKE_MODULE_PATH}")
endif(CLIP2TRI_IS_SUBBUILD)

#---------------------------------------------------------------------
# Define relative install locations.  Don't set these if they have already
# been set by some other means (like a higher level CMakeLists.txt file
# including this one).

include(${CLIP2TRI_CMAKE_DIR}/Path_Setup.cmake)

#---------------------------------------------------------------------
# The following logic is what allows binaries to run successfully in
# the build directory AND install directory.  Thanks to plplot for
# identifying the necessity of setting CMAKE_INSTALL_NAME_DIR on OSX.
# Documentation of these options is available at
# http://www.cmake.org/Wiki/CMake_RPATH_handling

include(${CLIP2TRI_CMAKE_DIR}/RPath_Setup.cmake)

#---------------------------------------------------------------------

if(NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif(NOT DEFINED BUILD_SHARED_LIBS)
if(NOT DEFINED BUILD_STATIC_LIBS)
  set(BUILD_STATIC_LIBS ON)
endif(NOT DEFINED BUILD_STATIC_LIBS)

#find_package(OpenGL)
#find_package(Threads)
# Use GLFW3 for visualizing/debugging/demo - just build one we know will work
#add_subdirectory(glfw)
#set(GLFW3_LIBRARIES glfw)
#set(GLFW3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)

include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(m cos "" HAVE_M_LIB)
if(HAVE_M_LIB)
  set(M_LIBRARY "m")
endif(HAVE_M_LIB)

#---------------------------------------------------------------------

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(poly2tri)
add_subdirectory(clipper)

if(MSVC)
  add_definitions("-DP2T_DLL_IMPORTS")
  add_definitions("-DCLIPPER_DLL_IMPORTS")
endif(MSVC)

if(BUILD_SHARED_LIBS)
  add_library(clip2tri clip2tri/clip2tri.cpp)
  target_link_libraries(clip2tri clipper poly2tri ${M_LIBRARY})
  install(TARGETS clip2tri
    RUNTIME DESTINATION ${BIN_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    ARCHIVE DESTINATION ${LIB_DIR})
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
  add_library(clip2tri-static clip2tri/clip2tri.cpp)
  target_link_libraries(clip2tri-static clpper-static poly2tri-static ${M_LIBRARY})
  install(TARGETS clip2tri-static
    RUNTIME DESTINATION ${BIN_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    ARCHIVE DESTINATION ${LIB_DIR})
endif(BUILD_STATIC_LIBS)


# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8

