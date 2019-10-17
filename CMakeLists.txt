cmake_minimum_required(VERSION 3.11)
project(raspberry_console)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES
    console/console.cpp
    console/main.cpp
    console/util.cpp
    console/controller/controller.cpp
    console/starship/column.cpp
    console/starship/ship_camera.cpp
    console/starship/ship.cpp
    console/starship/starship.cpp)

find_package(Threads)

add_definitions(-DBOOST_LOG_DYN_LINK -DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_ERROR_CODE_HEADER_ONLY)
find_package(
  Boost 1.67
  COMPONENTS log
  REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

# Don't download and compile raylib on Raspberry Pi because it doesn't work
# https://github.com/raysan5/raylib/issues/987
option(DOWNLOAD_RAYLIB "Download raylib if not present" OFF)
if(DOWNLOAD_RAYLIB)
  find_package(raylib 2.5 QUIET)

  if(NOT raylib_FOUND)
    include(FetchContent)

    FetchContent_Declare(raylib URL https://github.com/raysan5/raylib/archive/master.tar.gz)

    FetchContent_GetProperties(raylib)
    if(NOT raylib_POPULATED)
      set(FETCHCONTENT_QUIET NO)
      FetchContent_Populate(raylib)

      set(BUILD_EXAMPLES
          OFF
          CACHE BOOL "" FORCE)
      set(BUILD_GAMES
          OFF
          CACHE BOOL "" FORCE)

      add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
  endif()
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} Boost::log_setup Boost::log raylib)
