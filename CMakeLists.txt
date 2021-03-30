cmake_minimum_required(VERSION 3.11)
project(ReMania-GameBox VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# GLM

add_subdirectory(lib/glm)

# ENTT

add_subdirectory(lib/entt)

# Options

option(USE_OPENGL "Build RMGB-Graphics with OpenGL support" ON)
option(USE_MULTITHREADING "Build RMGB with support for using multiple CPU cores" ON)
option(PS3 "Build RMGB with support for the PS3 (Uses PSL1GHT) - Incompatible with OpenGL" OFF)
option(USE_SPU "Build RMGB with support for using the PS3 SPUs" OFF)
option(USE_TRACY "Build RMGB with support for profiling with Tracy" OFF)

# Option verification

if (PS3 MATCHES "ON" AND USE_OPENGL MATCHES "ON")
    message(FATAL_ERROR "Impossible configuration requested -- Building for PS3 and OpenGL at the same time is unsupported")
endif()

if (PS3 MATCHES "ON" AND USE_TRACY MATCHES "ON")
    message(FATAL_ERROR "Impossible configuration requested -- Tracy and the PS3 are not compatible")
endif()

if (PS3 MATCHES "OFF" AND USE_SPU MATCHES "ON")
    message(FATAL_ERROR "Impossible configuration requested -- You can't use SPUs whilst not being on a PS3")
endif()

# Utility variables

set(SRC_DIR src/RMGB)
set(INC_DIR include/RMGB)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Add libraries

if (PS3 MATCHES "OFF")
    # Use normal ImGUI
    add_library(RMGB-Graphics SHARED ${SRC_DIR}/Graphics/graphics.cpp
            lib/imgui/imgui.cpp lib/imgui/imgui_draw.cpp lib/imgui/imgui_tables.cpp lib/imgui/imgui_widgets.cpp
            lib/imgui/backends/imgui_impl_sdl.cpp)
else()
    # Use special ImGUI
    add_library(RMGB-Graphics SHARED ${SRC_DIR}/Graphics/graphics.cpp
            lib/imgui/imgui.cpp lib/imgui/imgui_draw.cpp lib/imgui/imgui_tables.cpp lib/imgui/imgui_widgets.cpp
            psl1ght-integration/imgui_impl_ps3.cpp)
endif()
add_library(RMGB-Physics SHARED ${SRC_DIR}/Physics/physics.cpp)

add_library(RMGB-Assets SHARED ${SRC_DIR}/Assets/assets.cpp)

if (USE_TRACY MATCHES "ON")
    add_library(RMGB-Utils SHARED ${SRC_DIR}/Utils/logger.cpp tracy/TracyClient.cpp)
    target_compile_definitions(RMGB-Utils PUBLIC TRACY_ENABLE)
else()
    add_library(RMGB-Utils SHARED ${SRC_DIR}/Utils/logger.cpp)
endif()

set(COMMON_LIB_INCLUDES ${CMAKE_SOURCE_DIR}/${INC_DIR} ${CMAKE_SOURCE_DIR}/lib lib/entt/single_include lib/glm tracy)

target_include_directories(RMGB-Graphics PUBLIC ${COMMON_LIB_INCLUDES} lib/imgui lib/imgui/backends)
target_include_directories(RMGB-Physics PUBLIC ${COMMON_LIB_INCLUDES})
target_include_directories(RMGB-Assets PUBLIC ${COMMON_LIB_INCLUDES})
target_include_directories(RMGB-Utils PUBLIC ${COMMON_LIB_INCLUDES})

target_link_libraries(RMGB-Graphics PUBLIC RMGB-Utils)
target_link_libraries(RMGB-Physics PUBLIC RMGB-Utils)
target_link_libraries(RMGB-Assets PUBLIC RMGB-Utils)

if(PS3 MATCHES "ON")
    include(psl1ght-integration/psl1ght.cmake)
    setupPSL1GHT(RMGB-Graphics)
    target_include_directories(RMGB-Graphics PUBLIC ${PSL1GHT}/portlibs/ppu/include/SDL)
    setupPSL1GHT(RMGB-Physics)
    setupPSL1GHT(RMGB-Assets)
    target_include_directories(RMGB-Utils PUBLIC ${PSL1GHT}/ppu/include/net)
    setupPSL1GHT(RMGB-Utils)
    target_compile_definitions(RMGB-Utils PUBLIC PS3)
endif()

if(USE_OPENGL MATCHES "ON")
    find_package(OpenGL 4.3 REQUIRED)
    find_package(GLEW 1.9)
    find_package(SDL2 QUIET)
    find_package(SDL2_image QUIET)
    target_sources(RMGB-Graphics PUBLIC src/RMGB/Graphics/backends/opengl.cpp lib/imgui/backends/imgui_impl_opengl3.cpp)
    target_compile_definitions(RMGB-Graphics PUBLIC OPENGL)
    if (GLEW_FOUND)
        target_link_libraries(RMGB-Graphics PUBLIC GLEW::GLEW)
    else()
        message(" -- Building GLEW from scratch")
        add_subdirectory(lib/glew)
        target_link_libraries(RMGB-Graphics libglew_shared)
    endif()
    if (SDL2_FOUND AND SDL2_IMAGE_FOUND)
        target_link_libraries(RMGB-Graphics PUBLIC ${SDL2_LIBRARIES})
        target_include_directories(RMGB-Graphics PUBLIC ${SDL2_INCLUDE_DIRS})
    else()
        message(" -- Building SDL2 from scratch")
        add_subdirectory("lib/sdl2")
        target_link_libraries(RMGB-Graphics PUBLIC SDL2)
        target_include_directories(RMGB-Graphics PUBLIC lib/sdl2/include)
    endif()
endif()

add_custom_target(ReMania-GameBox)
add_dependencies(ReMania-GameBox RMGB-Graphics RMGB-Physics RMGB-Assets)