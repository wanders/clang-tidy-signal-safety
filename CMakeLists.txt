
project(ExternalClangTidyModule LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

find_package(Clang REQUIRED CONFIG)

include_directories(${CLANG_INCLUDE_DIRS})

add_library(SignalSafetyClangTidyPlugin SHARED
  plugin.cpp
  )
