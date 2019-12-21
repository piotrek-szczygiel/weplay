cmake_minimum_required(VERSION 3.11)
project(weplay)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    console/Main.cpp
    console/Console.cpp
    console/Logging.cpp
    console/controller/Broadcaster.cpp
    console/controller/Network.cpp
    console/controller/Server.cpp
    console/menu/Menu.cpp
    console/pong/Pong.cpp
    console/pong/Player.cpp
    console/pong/Ball.cpp
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

if(WIN32)
    option(LOGGING "Enable logging" ON)
else()
    option(LOGGING "Enable logging" OFF)
endif()

if(LOGGING)
    add_compile_definitions(CONSOLE_LOGGING)
else()
    if(MSVC)
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE
            "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    endif()
endif()

if(WIN32)
    set(DOWNLOAD_RAYLIB ON)
else()
    option(DOWNLOAD_RAYLIB "Download Raylib" OFF)
endif()

if(DOWNLOAD_RAYLIB)
    find_package(raylib 2.5 QUIET)

    if(NOT raylib_FOUND)
        include(FetchContent)

        # My fork of raysan5's library without examples, games, projects and templates
        FetchContent_Declare(raylib
                             URL https://github.com/piotrek-szczygiel/raylib/archive/master.tar.gz)

        FetchContent_GetProperties(raylib)
        if(NOT raylib_POPULATED)
            set(FETCHCONTENT_QUIET NO)
            FetchContent_Populate(raylib)

            set(SHARED
                ON
                CACHE BOOL "" FORCE)

            add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
        endif()
    endif()
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/libraries)

if(WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib_static iphlpapi ws2_32)
else()
    find_package(Threads)
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib stdc++fs ${CMAKE_THREAD_LIBS_INIT})
endif()
