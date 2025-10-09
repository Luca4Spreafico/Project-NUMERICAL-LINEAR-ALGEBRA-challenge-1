cmake_minimum_required(VERSION 3.29)
project(Project_NUMERICAL_LINEAR_ALGEBRA_challenge_1)

set(CMAKE_CXX_STANDARD 20)

# Define the path to the LIS library
set(LIS_ROOT "\\\\wsl.localhost\\Ubuntu\\home\\luke\\lis-2.1.10")
include_directories("${LIS_ROOT}/include")
link_directories("${LIS_ROOT}/lib")

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})


# Add include directories for headers (Eigen + stb_image inside external/)
include_directories(${CMAKE_SOURCE_DIR}/external)


add_executable(Lab3
        Tutorial_Course/Lab3.cpp
)

add_executable(jacobi
        jacobi.cpp
)

add_executable(code2
        code2.cpp
)

target_link_libraries(code2 MPI::MPI_CXX)



