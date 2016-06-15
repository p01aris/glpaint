#========================================================================
#                      -  glpaint 0.1  - 
#------------------------------------------------------------------------
# Copyright (c) 2016-2017 Zheng Lu <polaris.varlin@gmail.com>
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would
#    be appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such, and must not
#    be misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any source
#    distribution.
#
#========================================================================
cmake_minimum_required(VERSION 2.6)
PROJECT(glpaint)

# Usage: 
#   cmake .. -DCMAKE_INCLUDE_PATH=/home/ott/exp/include -DCMAKE_LIBRARY_PATH=/home/ott/exp/lib
#       -DBOOST_INCLUDEDIR=/home/ott/exp/include -DBOOST_LIBRARYDIR=/home/ott/exp/lib

#Check CXX11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(OpenGL)
FIND_LIBRARY(COCOA_LIBRARY Cocoa)
if (OPENGL_FOUND)
  MESSAGE("OpenGL Correctly Found")
  include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
  MESSAGE("OpenGL environment missing")
endif()

option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)

find_package(GLFW)
find_package(OpenGL REQUIRED)
#SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_REGEX_LIBRARY})
include_directories (${GLFW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARIES} ${GLFW_STATIC_LIBRARIES})
add_subdirectory(ext/glfw)
include_directories(ext/glfw/include)

ADD_EXECUTABLE(main main.cpp glpaint_foo.hpp glpaint.cpp)
TARGET_LINK_LIBRARIES(main glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${COCOA_LIBRARY})

#ADD_EXECUTABLE(test-mcmt test-mcmt.cpp)
#TARGET_LINK_LIBRARIES(test-mcmt ${USED_LIBS})

#ADD_EXECUTABLE(test-otpc-tp test-otpc-tp.cpp test-otpc-conn.cpp)
#TARGET_LINK_LIBRARIES(test-otpc-tp ${USED_LIBS})