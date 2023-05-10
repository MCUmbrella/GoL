cmake_minimum_required(VERSION 3.18)
project(GoL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
file(GLOB SOURCES src/* src/*/*)

add_compile_options(-Wall -Werror=maybe-uninitialized)

Add_Executable (${CMAKE_PROJECT_NAME} ${SOURCES})
