cmake_minimum_required(VERSION 3.12)

project(imagine
    LANGUAGES C
    DESCRIPTION "Collection of image processing algorithms")

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
# set(BUILD_SHARED_LIBS OFF)
# set(CMAKE_EXE_LINKER_FLAGS "-static")

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

add_definitions(
  -DSUPPORT_FILEFORMAT_BMP=1
  -DSUPPORT_FILEFORMAT_TGA=1
  -DSUPPORT_FILEFORMAT_JPG=1
  -DSUPPORT_FILEFORMAT_GIF=1
  -DSUPPORT_FILEFORMAT_QOI=1
  -DSUPPORT_FILEFORMAT_PSD=1
  -DSUPPORT_FILEFORMAT_DDS=1
  -DSUPPORT_FILEFORMAT_HDR=1
  -DSUPPORT_FILEFORMAT_PIC=1
  -DSUPPORT_FILEFORMAT_KTX=1
  -DSUPPORT_FILEFORMAT_ASTC=1
  -DSUPPORT_FILEFORMAT_PKM=1
  -DSUPPORT_FILEFORMAT_PVR=1
  -DSUPPORT_FILEFORMAT_SVG=1)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "git@github.com:raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
    GIT_SHALLOW 1
)

FetchContent_MakeAvailable(raylib)


if (APPLE)
  list(APPEND FRAMEWORKS "-framework CoreVideo")
  list(APPEND FRAMEWORKS "-framework IOKit")
  list(APPEND FRAMEWORKS "-framework Cocoa")
  list(APPEND FRAMEWORKS "-framework GLUT")
  list(APPEND FRAMEWORKS "-framework OpenGL")
  list(JOIN FRAMEWORKS " " FRAMEWORKS)
  set(CMAKE_C_FLAGS ${FRAMEWORKS})
elseif (LINUX)
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

add_executable(dots "${SOURCE_DIR}/dots.c")
target_include_directories(dots PRIVATE "${SOURCE_DIR}")
target_link_libraries(dots PRIVATE raylib)

if(APPLE)
  add_executable(dots_app MACOSX_BUNDLE "${SOURCE_DIR}/dots.c")
  target_include_directories(dots_app PRIVATE "${SOURCE_DIR}")
  target_link_libraries(dots_app PRIVATE raylib)

  # Bundling macOS application
  set_target_properties(dots_app
    PROPERTIES
      BUNDLE True
      MACOSX_BUNDLE_BUNDLE_NAME Dots
      MACOSX_BUNDLE_BUNDLE_VERSION "0.0.1"
      MACOSX_BUNDLE_GUI_IDENTIFIER org.nk2ge5k.imagine.dots
      MACOSX_BUNDLE_ICON_FILE AppIcon
      MACOSX_BUNDLE_SHORT_VERSION_STRING "0.0.1"
  )
endif()
