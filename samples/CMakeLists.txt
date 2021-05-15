cmake_minimum_required(VERSION 3.11)
project(RMGB-Samples CXX)

file(GLOB Samples */)
foreach (sample ${Samples})
    if (NOT ${sample} MATCHES ".+CMakeLists.txt")
        add_subdirectory(${sample})
    endif()
endforeach()