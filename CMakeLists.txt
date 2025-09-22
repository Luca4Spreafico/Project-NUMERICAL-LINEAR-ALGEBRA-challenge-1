cmake_minimum_required(VERSION 3.29)
project(Project_NUMERICAL_LINEAR_ALGEBRA_challenge_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

# Add include directories for headers (Eigen + stb_image inside external/)
include_directories(${CMAKE_SOURCE_DIR}/external)

add_executable(Project_NUMERICAL_LINEAR_ALGEBRA_challenge_1
    attempt1.cpp
)
