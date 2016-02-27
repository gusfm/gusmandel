cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME gusmandel)
project(${PROJECT_NAME} C CXX)

find_package(Threads REQUIRED)

set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -std=c11 -O3")
#set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -std=c11 -g")
#set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -std=c11 -pg")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O3")
include_directories(${PROJECT_SOURCE_DIR}/src)

#-------------------
# Module source
#-------------------

add_executable(${PROJECT_NAME}
    src/image.c
    src/rgb.c
    src/palette.c
    src/ppm.c
    src/mandel.c
    src/gusmandel.c
    src/main.c)

target_link_libraries(${PROJECT_NAME} m pthread)


#-------------------
# Test
#-------------------
#enable_testing()
#set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)
#
#add_executable(${PROJECT_TEST_NAME}
#    test/cpu_opcodes_test.cc
#    )
#
#target_link_libraries(${PROJECT_TEST_NAME}
#    /usr/lib/libgtest.a
#    /usr/lib/libgtest_main.a
#    ${CMAKE_THREAD_LIBS_INIT}
#    libcpu
#    GL 
#    )
#
#add_dependencies(${PROJECT_TEST_NAME} googletest)
#
#add_test(test ${PROJECT_TEST_NAME})

# Copy valgrind test script to build directory
file(COPY scripts/valgrind_test.sh DESTINATION .)
