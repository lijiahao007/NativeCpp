cmake_minimum_required(VERSION 3.22.1)
project("nativecpp")

add_subdirectory(Utils)

file(GLOB CPP_SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${CMAKE_PROJECT_NAME} SHARED
        ${CPP_SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        Utils)  # 添加Utils子模块的链接


set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
)