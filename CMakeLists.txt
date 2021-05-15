cmake_minimum_required(VERSION 3.11)
project(ReMania-GameBox VERSION 0.0.1 LANGUAGES CXX)

if(PS3)
    include(psl1ght-integration/psl1ght.cmake)
endif()

set(CMAKE_CXX_STANDARD 20)

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
option(BUILD_SAMPLES "Build samples" ON)
option(BUILD_DOCS "Build documentation" OFF)

# Option verification

if(PS3 MATCHES "ON" AND CMAKE_TOOLCHAIN_FILE STREQUAL "")
    message(FATAL_ERROR "I honestly don't know how you managed this but you've enabled PS3 whilst disabling the CMAKE_TOOLCHAIN_FILE")
endif()

if(PS3 MATCHES "OFF" AND CMAKE_SYSTEM_NAME STREQUAL "PS3")
    message(FATAL_ERROR "I honestly don't know how you managed this but you've disabled PS3 whilst enabling the CMAKE_TOOLCHAIN_FILE")
endif()

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

cmake_policy(SET CMP0076 NEW)

if(BUILD_DOCS)
    include(Docs/Doxygen.cmake)
endif()

# Add libraries
if (NOT PS3)
    add_library(RMGB-Graphics SHARED ${SRC_DIR}/Graphics/graphics.cpp
                lib/imgui-ocornut/imgui.cpp lib/imgui-ocornut/imgui_draw.cpp lib/imgui-ocornut/imgui_tables.cpp lib/imgui-ocornut/imgui_widgets.cpp
                lib/imgui-ocornut/backends/imgui_impl_sdl.cpp)

    add_library(RMGB-Physics SHARED ${SRC_DIR}/Physics/physics.cpp)

    add_library(RMGB-Assets SHARED ${SRC_DIR}/Assets/assets.cpp)

    if (USE_TRACY MATCHES "ON")
        add_library(RMGB-Utils SHARED ${SRC_DIR}/Utils/logger.cpp tracy/TracyClient.cpp)
        target_compile_definitions(RMGB-Utils PUBLIC TRACY_ENABLE)
    else()
        add_library(RMGB-Utils SHARED ${SRC_DIR}/Utils/logger.cpp)
    endif()
else()
    add_library(RMGB-Graphics STATIC ${SRC_DIR}/Graphics/graphics.cpp
            lib/imgui-ocornut/imgui.cpp lib/imgui-ocornut/imgui_draw.cpp lib/imgui-ocornut/imgui_tables.cpp lib/imgui-ocornut/imgui_widgets.cpp
            lib/imgui-ocornut/backends/imgui_impl_sdl.cpp)
    setupPSL1GHT_library(RMGB-Graphics)
    add_library(RMGB-Physics STATIC ${SRC_DIR}/Physics/physics.cpp)
    setupPSL1GHT_library(RMGB-Physics)
    add_library(RMGB-Assets STATIC ${SRC_DIR}/Assets/assets.cpp)
    setupPSL1GHT_library(RMGB-Assets)
    add_library(RMGB-Utils STATIC ${SRC_DIR}/Utils/logger.cpp)
    setupPSL1GHT_library(RMGB-Utils)
    target_include_directories(RMGB-Graphics PUBLIC ${PSL1GHT}/portlibs/ppu/include/SDL2)
    target_compile_definitions(RMGB-Graphics PUBLIC ON_PS3)
    target_compile_definitions(RMGB-Physics PUBLIC ON_PS3)
    target_compile_definitions(RMGB-Assets PUBLIC ON_PS3)
    target_include_directories(RMGB-Utils PUBLIC ${PSL1GHT}/ppu/include/net)
    target_compile_definitions(RMGB-Utils PUBLIC ON_PS3)

    target_sources(RMGB-Graphics PUBLIC src/RMGB/Graphics/backends/ps3.cpp)
    target_include_directories(RMGB-Graphics PUBLIC include/RMGB/Graphics/backends/)
endif()

set(COMMON_LIB_INCLUDES ${CMAKE_SOURCE_DIR}/${INC_DIR} ${CMAKE_SOURCE_DIR}/lib lib/entt/single_include lib/glm tracy)

target_include_directories(RMGB-Graphics PUBLIC ${COMMON_LIB_INCLUDES} lib/imgui-ocornut lib/imgui-ocornut/backends)
target_include_directories(RMGB-Physics PUBLIC ${COMMON_LIB_INCLUDES})
target_include_directories(RMGB-Assets PUBLIC ${COMMON_LIB_INCLUDES})
target_include_directories(RMGB-Utils PUBLIC ${COMMON_LIB_INCLUDES})

target_link_libraries(RMGB-Graphics PUBLIC RMGB-Utils)
target_link_libraries(RMGB-Physics PUBLIC RMGB-Utils)
target_link_libraries(RMGB-Assets PUBLIC RMGB-Utils)

if(USE_OPENGL)
    find_package(OpenGL 4.3 REQUIRED)
    find_package(GLEW 1.9)
    find_package(SDL2 QUIET)
    find_package(SDL2_image QUIET)
    target_sources(RMGB-Graphics PUBLIC src/RMGB/Graphics/backends/opengl.cpp lib/imgui-ocornut/backends/imgui_impl_opengl3.cpp)
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
        add_subdirectory("lib/sdl")
        target_link_libraries(RMGB-Graphics PUBLIC SDL2)
        target_include_directories(RMGB-Graphics PUBLIC lib/sdl/include)
    endif()
endif()

add_custom_target(ReMania-GameBox)
add_dependencies(ReMania-GameBox RMGB-Graphics RMGB-Physics RMGB-Assets)

if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
