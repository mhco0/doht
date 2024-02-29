# Doth Libray (C) by Marcos Oliveira

cmake_minimum_required(VERSION 3.20)

if(NOT CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE
      "${CMAKE_CURRENT_LIST_DIR}/3rd_party/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(doht LANGUAGES C CXX)

add_library(shc STATIC "include/doht/shc.h")
target_sources(shc PUBLIC "src/shc.cpp")
target_include_directories(shc PUBLIC "include")

find_package(GTest CONFIG REQUIRED)
add_subdirectory(tests)

add_subdirectory(tools)
