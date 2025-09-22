cmake_minimum_required(VERSION 3.29)
project(Project_NUMERICAL_LINEAR_ALGEBRA_challenge_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project_NUMERICAL_LINEAR_ALGEBRA_challenge_1
    attempt1.cpp
        stb_image.h
        external/fe.cpp)
