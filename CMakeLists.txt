cmake_minimum_required(VERSION 4.0)

set(PROJECT_NAME ZHELL)
project(${PROJECT_NAME})

set(SOURCES src/main.cpp src/parser.cpp)

add_executable(zhell ${SOURCES})
target_include_directories(zhell PUBLIC src/)