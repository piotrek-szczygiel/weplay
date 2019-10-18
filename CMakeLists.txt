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

if(NOT WIN32)
    add_definitions(-DBOOST_LOG_DYN_LINK -DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_ERROR_CODE_HEADER_ONLY)
endif()

find_package(
    Boost 1.67
    COMPONENTS log_setup log
    REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

# Don 't download raylib on Raspberry Pi because it doesn' t compile https://github.com/raysan5/raylib/issues/987
option(DOWNLOAD_RAYLIB "Download raylib if not present" OFF)
if(WIN32)
    set(DOWNLOAD_RAYLIB
        ON
        CACHE BOOL "" FORCE)
endif()

if(DOWNLOAD_RAYLIB)
    find_package(raylib 2.5 QUIET)

    if(NOT raylib_FOUND)
        include(FetchContent)

        # My fork of original library that fixes symbol collisions on Windows
        FetchContent_Declare(raylib URL https://github.com/piotrek-szczygiel/raylib/archive/master.tar.gz)

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

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} Boost::log_setup Boost::log raylib_static)

# Link against winmm.lib on Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm)
endif()

# Windows shenanigans
if(WIN32)
    macro(get_win32_winnt version)
        if(CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
            string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
            if("${verMajor}" MATCHES "10")
                set(verMajor "A")
                string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
            endif("${verMajor}" MATCHES "10")
            string(REPLACE "." "" ver ${ver})
            string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
            set(${version} "0x${ver}")
        endif(CMAKE_SYSTEM_VERSION)
    endmacro(get_win32_winnt)

    get_win32_winnt(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
endif(WIN32)
