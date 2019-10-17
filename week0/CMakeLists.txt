cmake_minimum_required(VERSION 2.9 )

project( Week0 )
include_directories("/usr/include/eigen3")
find_package(Sophus REQUIRED)
include_directories(${Sophus_INCLUDE_DIRS})
add_executable( Week0 R_Quaternion_update.cpp)