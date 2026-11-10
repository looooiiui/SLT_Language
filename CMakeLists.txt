cmake_minimum_required(VERSION 3.16)
# 项目名字在这里
project(slt_language)
# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 批量获取 includes下面头文件
file(GLOB_RECURSE HEADER_FILES
    "${PROJECT_SOURCE_DIR}/includes/*.h"
    "${PROJECT_SOURCE_DIR}/includes/*.hpp"
)
# 批量获取 src下面源文件
file(GLOB_RECURSE SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# 将所有文件添加到可执行文件中
add_executable(
    slt_language
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

# 这里可以添加 include 目录
target_include_directories(slt_language PRIVATE ${PROJECT_SOURCE_DIR}/includes)