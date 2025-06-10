# 设置最低要求的 CMake 版本
cmake_minimum_required(VERSION 3.30)

# 定义这个模块的名称
project(pikaPackage)

# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR}/pikascript-api ${PROJECT_SOURCE_DIR}/pikascript-core ${PROJECT_SOURCE_DIR}/pikascript-lib)

# 收集所有源文件（C、C++ 和 头文件）
file(GLOB_RECURSE PIKAPACKAGE_SOURCE_FILES
    "*.c"
    "*.cpp"
    "*.h"
)

# 将 pikaPackage 构建为静态库
add_library(pikaPackage STATIC ${PIKAPACKAGE_SOURCE_FILES})
