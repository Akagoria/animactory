cmake_minimum_required(VERSION 3.7)

project(ANIMACTORY
  VERSION 0.1.0
  LANGUAGES CXX
)

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS FALSE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

find_package(gfcore REQUIRED)
find_package(Cairo REQUIRED)


if(MSVC)
  message(STATUS "Using MSVC compiler")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX /W4 /utf-8 /std:c++14 /permissive-")
  # for more information on the last two switches:
  # - https://blogs.msdn.microsoft.com/vcblog/2016/06/07/standards-version-switches-in-the-compiler/
  # - https://blogs.msdn.microsoft.com/vcblog/2016/11/16/permissive-switch/
else()
  message(STATUS "Using GCC or Clang compiler")
  # assume it's a GNU-like compiler (GCC, clang)
  set(CMAKE_CXX_FLAGS                 "-Wall -Wextra -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG           "-O0 -g")
  set(CMAKE_CXX_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE         "-O3 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")
endif()

# library animactory0

add_library(animactory0
  library/Canvas.cc
  library/Paint.cc
  library/Path.cc
  library/Pattern.cc
  library/Surface.cc
)

target_include_directories(animactory0
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${CAIRO_INCLUDE_DIRS}
)

target_link_libraries(animactory0
  PUBLIC
    gf::gfcore0
    ${CAIRO_LIBRARIES}
)

set_target_properties(animactory0
  PROPERTIES
  VERSION ${ANIMACTORY_VERSION}
  SOVERSION ${ANIMACTORY_VERSION_MAJOR}
)

# binaries

add_executable(ay_deciduous_tree
  binary/ay_deciduous_tree.cc
)

target_link_libraries(ay_deciduous_tree
  animactory0
)
