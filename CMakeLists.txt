cmake_minimum_required(VERSION 3.10)
project(lvgl)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})


file(GLOB_RECURSE INCLUDES "lv_drivers/*.h" "lvgl/demos/*.h" "lvgl/*.h" "./*.h" "src/*.h")
file(GLOB_RECURSE SOURCES "lv_drivers/*.c" "lvgl/demos/*.c" "lvgl/src/*.c" "src/*.c" "src/*.cpp")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(CMAKE_CXX_FLAGS "-O3")


add_executable(main main.c mouse_cursor_icon.c ${SOURCES} ${INCLUDES}
)
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)
add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)

# 指定 SDL2 的安装路径
set(SDL2_DIR "${PROJECT_SOURCE_DIR}/SDL2/SDL2-devel-2.30.3-mingw/SDL2-2.30.3/x86_64-w64-mingw32")
set(SDL2_TTF_DIR "${PROJECT_SOURCE_DIR}/SDL2/SDL2_ttf-2.22.0")
set(SDL2_IMAGE_DIR "${PROJECT_SOURCE_DIR}/SDL2/SDL_image-SDL2")
# 包含 SDL2 的头文件目录
include_directories(
        ${SDL2_DIR}/include
        ${SDL2_TTF_DIR}/include
        ${SDL2_IMAGE_DIR}/include
)
# 链接 SDL2 库和 SDL2main 库
target_link_libraries(main
        "${SDL2_DIR}/lib/libSDL2.dll.a"
        "${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.lib"
        "${SDL2_IMAGE_DIR}/SDL2_image-2.8.4-win32-x64/SDL2_image.dll"
)