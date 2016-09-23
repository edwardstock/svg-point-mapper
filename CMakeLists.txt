cmake_minimum_required(VERSION 3.5.0)
project(FontMapper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")
set(SOURCE_FILES main.cpp libs/nanosvg/src/nanosvg.h libs/nanosvg/src/nanosvgrast.h parser/cmdline.h)

set(NANOSVG_INCLUDE_DIRS libs/nanosvg/src)
include_directories(${NANOSVG_INCLUDE_DIRS})

add_executable(FontMapper ${SOURCE_FILES})

