cmake_minimum_required(VERSION 3.10)
project(NullSpaceTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include_directories(“D:\\eigen-git-mirror-3.2.9\\”)
find_package(Eigen3 REQUIRED eigen3)
#find_package(Sophus REQUIRED)

include_directories(
#        ${EIGEN3_INCLUDE_DIR}
##        ${Sophus_INCLUDE_DIRS}
)


add_executable(NullSpaceTest hessian_nullspace_test.cpp)
#target_link_libraries(NullSpaceTest  ${Sophus_LIBRARIES})
