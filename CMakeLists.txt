cmake_minimum_required(VERSION 3.11)
project(raspberry_console)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    console/Main.cpp
    console/Console.cpp
    console/controller/Controller.cpp
    console/controller/TcpSession.cpp
    console/controller/TcpServer.cpp
    console/menu/Menu.cpp
    console/starship/Starship.cpp
    console/starship/Ship.cpp
    console/starship/ShipCamera.cpp
    console/starship/Column.cpp
    console/tetris/Bag.cpp
    console/tetris/DrawBlock.cpp
    console/tetris/Input.cpp
    console/tetris/Matrix.cpp
    console/tetris/Piece.cpp
    console/tetris/Player.cpp
    console/tetris/Score.cpp
    console/tetris/Shape.cpp
    console/tetris/Tetris.cpp
    console/Util.cpp)

find_package(Threads)

if(NOT WIN32)
    add_definitions(-DBOOST_LOG_DYN_LINK -DBOOST_SYSTEM_NO_DEPRECATED
                    -DBOOST_ERROR_CODE_HEADER_ONLY)
endif()

find_package(
    Boost 1.67
    COMPONENTS log_setup log
    REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

option(RASPBERRY_PI "Compile for Raspberry Pi" OFF)
if(NOT RASPBERRY_PI)
    find_package(raylib 2.5 QUIET)

    if(NOT raylib_FOUND)
        include(FetchContent)

        # My fork of original library that fixes symbol collisions on Windows
        FetchContent_Declare(raylib
                             URL https://github.com/piotrek-szczygiel/raylib/archive/master.tar.gz)

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

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} Boost::log_setup Boost::log)

if(RASPBERRY_PI)
    target_link_libraries(${PROJECT_NAME} raylib)
else()
    target_link_libraries(${PROJECT_NAME} raylib_static)
endif()

if(NOT WIN32)
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()

# Link against winmm.lib on Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm)

    # Set _WIN32_WINNT so Boost doesn't complain
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
