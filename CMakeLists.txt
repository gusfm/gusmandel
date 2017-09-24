cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME gusmandel)
project(${PROJECT_NAME} C CXX)

find_package(Threads REQUIRED)

set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -std=c11 -O3")
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}
    src/image.c
    src/rgb.c
    src/palette.c
    src/ppm.c
    src/mandel.c
    src/gusmandel.c
    src/main.c)

target_link_libraries(${PROJECT_NAME} m pthread)
