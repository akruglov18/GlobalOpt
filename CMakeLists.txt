cmake_minimum_required(VERSION 3.12)
project(GlobalOptimization)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable (GlobalOptimization main.cpp)
add_subdirectory(Base)
add_subdirectory(AGP)
add_subdirectory(Kushner)
target_link_libraries(GlobalOptimization AGP)