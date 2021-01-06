cmake_minimum_required(VERSION 3.13)
project(Ovs)

set(CMAKE_CXX_STANDARD 17)

add_executable(Ovs OvsLang.cpp)

SET(EXECUTABLE_OUTPUT_PATH "../")