cmake_minimum_required(VERSION 3.10)
project(rasterisation)

set(CMAKE_CXX_STANDARD 14)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(rasterisation rdv.cpp model.cpp model.h data.hpp matrix.h matrix.cpp)
