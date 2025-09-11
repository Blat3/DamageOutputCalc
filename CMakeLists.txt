cmake_minimum_required(VERSION 4.0)
project(DamageOutputCalc)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(DamageOutputCalc main.cpp)
