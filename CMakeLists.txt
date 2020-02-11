cmake_minimum_required(VERSION 3.10)
project(ProjetRDV)

set(CMAKE_CXX_STANDARD 14)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(ProjetRDV rdv.cpp model.cpp model.h)
