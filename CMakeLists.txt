cmake_minimum_required(VERSION 3.12)

project(imagine
    LANGUAGES C
    DESCRIPTION "Collection of image processing algorithms")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(CMAKE_C_FLAGS "-framework Cocoa -framework IOKit -framework OpenGL")

if(UNIX)
    add_compile_options(
        -Wall
        -Wno-unknown-pragmas
        -Wextra
        -Wshadow
        -Winit-self
        -Wpedantic
        $<$<COMPILE_LANGUAGE:C>:-Wmissing-prototypes>)
    add_definitions(-D_GNU_SOURCE)
endif()

set(CMAKE_FIND_DEBUG_MODE TRUE)
find_package(raylib REQUIRED)

## DOTS

add_executable(dots "${SOURCE_DIR}/dots.c" "${SOURCE_DIR}/types.c")
target_include_directories(dots PRIVATE "${SOURCE_DIR}")
target_link_libraries(dots PRIVATE raylib)
