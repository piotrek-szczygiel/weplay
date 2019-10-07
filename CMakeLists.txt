cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(raspberry-console)

if("${PLATFORM}" MATCHES "PLATFORM_RPI")
    link_directories(/opt/vc/lib)
    link_directories(${PROJECT_SOURCE_DIR}/raylib/raspberry)
    include_directories(
        /opt/vc/include
        /opt/vc/include/interface/vmcs_host/linux
        /opt/vc/include/interface/vcos/pthreads
    )
endif()

if(WIN32)
    link_directories(${PROJECT_SOURCE_DIR}/raylib/windows)
endif()

include_directories(${PROJECT_SOURCE_DIR}/raylib)

add_executable(${PROJECT_NAME} src/main.cc)

target_link_libraries(${PROJECT_NAME} raylib)

if("${PLATFORM}" MATCHES "PLATFORM_RPI")
    target_link_libraries(${PROJECT_NAME} brcmGLESv2 brcmEGL pthread rt m bcm_host dl)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm)
endif()
