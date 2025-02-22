# 1. 定义子模块的最小CMake版本要求
cmake_minimum_required(VERSION 3.22.1)

# 2. 声明独立子模块（使用父项目的项目名称避免冲突）
project(${CMAKE_PROJECT_NAME}_Utils LANGUAGES CXX)

# 3. 收集当前目录（Utils）下的所有cpp文件
file(GLOB UTILS_SOURCES CONFIGURE_DEPENDS "*.cpp")

# 4. 创建静态库目标（推荐子模块使用静态链接）
add_library(Utils STATIC
    ${UTILS_SOURCES}
)

# 5. 设置包含路径（暴露头文件给父项目）
target_include_directories(Utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 6. 设置C++标准版本（保持与父项目一致）
set_target_properties(Utils PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)
