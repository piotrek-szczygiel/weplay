cmake_minimum_required(VERSION 3.11)
project(raspberry_console)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE
  console/console.cpp
  console/main.cpp
  console/util.cpp
  console/controller/controller.cpp
  console/starship/column.cpp
  console/starship/ship_camera.cpp
  console/starship/ship.cpp
  console/starship/starship.cpp
)

find_package(Threads)

find_package(Boost 1.71.0 REQUIRED COMPONENTS date_time regex)
include_directories(${Boost_INCLUDE_DIRS})

find_package(raylib 2.5 QUIET)

if (NOT raylib_FOUND)
  include(FetchContent)

  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/master.tar.gz
  )

  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)

    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

  endif()
endif()

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
  Boost::date_time
  raylib
)
