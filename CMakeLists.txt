cmake_minimum_required(VERSION 3.11)
set (CMAKE_CXX_STANDARD 17)

project(raspberry-console)

if("${PLATFORM}" MATCHES "PLATFORM_RPI")
    link_directories(/opt/vc/lib)
    include_directories(
        /opt/vc/include
        /opt/vc/include/interface/vmcs_host/linux
        /opt/vc/include/interface/vcos/pthreads
    )
endif()

link_directories(
    ${PROJECT_SOURCE_DIR}/raylib
    ${PROJECT_SOURCE_DIR}/raylib/src
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} raylib)

if("${PLATFORM}" MATCHES "PLATFORM_RPI")
    target_link_libraries(${PROJECT_NAME} brcmGLESv2 brcmEGL pthread rt m bcm_host dl)
endif()
