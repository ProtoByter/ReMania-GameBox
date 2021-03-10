cmake_minimum_required(VERSION 3.11)
project(ReMania-GameBox VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# GLM

add_subdirectory(lib/glm)

# ENTT

add_subdirectory(lib/entt)

# Options

option(USE_OPENGL "Build RMGB-Graphics with OpenGL support" ON)
option(USE_MULTITHREADING "Build RMGB with support for using multiple CPU cores" ON)

# Utility variables

set(SRC_DIR src/RMGB)
set(INC_DIR include/RMGB)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Add libraries

add_library(RMGB-Graphics SHARED ${SRC_DIR}/Graphics/graphics.cpp)
add_library(RMGB-Physics SHARED ${SRC_DIR}/Physics/physics.cpp)
add_library(RMGB-Assets SHARED ${SRC_DIR}/Assets/assets.cpp)
add_library(RMGB-Utils SHARED ${SRC_DIR}/Utils/logger.cpp)

set(COMMON_LIB_INCLUDES ${CMAKE_SOURCE_DIR}/${INC_DIR} ${CMAKE_SOURCE_DIR}/lib lib/entt/single_include lib/glm)

target_include_directories(RMGB-Graphics PUBLIC ${COMMON_LIB_INCLUDES})
target_include_directories(RMGB-Physics PUBLIC ${COMMON_LIB_INCLUDES})
target_include_directories(RMGB-Assets PUBLIC ${COMMON_LIB_INCLUDES})
target_include_directories(RMGB-Utils PUBLIC ${COMMON_LIB_INCLUDES})

target_link_libraries(RMGB-Graphics RMGB-Utils)
target_link_libraries(RMGB-Physics RMGB-Utils)
target_link_libraries(RMGB-Assets RMGB-Utils)

if(USE_OPENGL MATCHES "ON")
    find_package(OpenGL 4.3 REQUIRED)
    find_package(GLEW 1.9)
    find_package(SDL2 QUIET)
    find_package(SDL2_image QUIET)
    target_compile_definitions(RMGB-Graphics PUBLIC OPENGL)
    if (GLEW_FOUND)
        target_link_libraries(RMGB-Graphics GLEW::GLEW)
    else()
        message(" -- Building GLEW from scratch")
        add_subdirectory(lib/glew)
        target_link_libraries(RMGB-Graphics libglew_shared)
    endif()
    if (SDL2_FOUND AND SDL2_IMAGE_FOUND)
        target_link_libraries(RMGB-Graphics ${SDL2_LIBRARIES})
        target_include_directories(RMGB-Graphics PUBLIC ${SDL2_INCLUDE_DIRS})
    else()
        message(" -- Building SDL2 from scratch")
        add_subdirectory("lib/sdl2")
        target_link_libraries(RMGB-Graphics SDL2)
        target_include_directories(RMGB-Graphics PUBLIC lib/sdl2/include)
    endif()
endif()

add_custom_target(ReMania-GameBox)
add_dependencies(ReMania-GameBox RMGB-Graphics RMGB-Physics RMGB-Assets)