cmake_minimum_required(VERSION 3.11)
project(RMGB-SimpleSample CXX)

include(RMGB-Helper.cmake)

add_executable(simple_sample main.cpp)
setupRMGB(simple_sample)