cmake_minimum_required(VERSION 3.15)
project(ProjetRDV)

set(CMAKE_CXX_STANDARD 14)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_executable(ProjetRDV rdv.cpp)

set(STB_DIR "${LIB_DIR}/stb")
target_include_directories(${PROJECT_NAME} PRIVATE "${STB_DIR}")