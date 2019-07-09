cmake_minimum_required(VERSION 3.14)
project(test C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(test main.c)