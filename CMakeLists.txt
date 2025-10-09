cmake_minimum_required(VERSION 3.29)
project(Project_NUMERICAL_LINEAR_ALGEBRA_challenge_1)

set(CMAKE_CXX_STANDARD 20)

# Define the path to the LIS library
set(LIS_ROOT "\\\\wsl.localhost\\Ubuntu\\home\\luke\\lis-2.1.10")
include_directories("${LIS_ROOT}/include")
link_directories("${LIS_ROOT}/lib")

# Add include directories for headers (Eigen + stb_image inside external/)
include_directories(${CMAKE_SOURCE_DIR}/external)

#add_executable(Luke
#    LukeChallenge1.cpp
#        external/stb_image.h
#        external/stb_image_write.h
#)

#add_executable(girlsCode
 #       girlsCode.cpp
 #       external/stb_image.h
 #       external/stb_image_write.h
#)

add_executable(Lab3
        Tutorial_Course/Lab3.cpp
)

add_executable(jacobi
        jacobi.cpp
)

add_executable(code2
        code2.cpp
)





