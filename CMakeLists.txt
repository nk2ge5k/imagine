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

add_compile_definitions(SUPPORT_FILEFORMAT_BMP=1)
add_compile_definitions(SUPPORT_FILEFORMAT_TGA=1)
add_compile_definitions(SUPPORT_FILEFORMAT_JPG=1)
add_compile_definitions(SUPPORT_FILEFORMAT_GIF=1)
add_compile_definitions(SUPPORT_FILEFORMAT_QOI=1)
add_compile_definitions(SUPPORT_FILEFORMAT_PSD=1)
add_compile_definitions(SUPPORT_FILEFORMAT_DDS=1)
add_compile_definitions(SUPPORT_FILEFORMAT_HDR=1)
add_compile_definitions(SUPPORT_FILEFORMAT_PIC=1)
add_compile_definitions(SUPPORT_FILEFORMAT_KTX=1)
add_compile_definitions(SUPPORT_FILEFORMAT_ASTC=1)
add_compile_definitions(SUPPORT_FILEFORMAT_PKM=1)
add_compile_definitions(SUPPORT_FILEFORMAT_PVR=1)
add_compile_definitions(SUPPORT_FILEFORMAT_SVG=1)

# Adding Raylib
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "git@github.com:raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
    GIT_SHALLOW 1
)

FetchContent_GetProperties(raylib)
if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
  set(FETCHCONTENT_QUIET NO)
  FetchContent_MakeAvailable(raylib)
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
endif()

if (UNIX)
  add_compile_options(
      -Wall
      -Wno-unknown-pragmas
      -Wextra
      -Wshadow
      -Winit-self
      -Wpedantic
      $<$<COMPILE_LANGUAGE:C>:-Wmissing-prototypes>)
  add_definitions(-D_GNU_SOURCE)
endif ()

set(SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(PROJECT_NAME "dots")

if(APPLE)
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE "${SOURCE_DIR}/dots.c")
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit")
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
  set_target_properties(${PROJECT_NAME}
    PROPERTIES
      BUNDLE True
      MACOSX_BUNDLE_BUNDLE_NAME Dots
      MACOSX_BUNDLE_BUNDLE_VERSION "0.0.1"
      MACOSX_BUNDLE_GUI_IDENTIFIER org.nk2ge5k.imagine.dots
      MACOSX_BUNDLE_ICON_FILE AppIcon
      MACOSX_BUNDLE_SHORT_VERSION_STRING "0.0.1"
  )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "${SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
