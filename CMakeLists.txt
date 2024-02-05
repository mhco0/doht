# Doth Libray (C) by Marcos Oliveira

cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Doht LANGUAGES C CXX)

add_library(hello STATIC "include/doht/hello.h")
target_sources(hello PUBLIC "src/hello.cpp")
target_include_directories(hello PUBLIC "include")

find_package(GTest CONFIG REQUIRED)
add_subdirectory(tests)
