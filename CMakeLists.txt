project(time_bench)
cmake_minimum_required(VERSION 2.6)

add_definitions("-D_POSIX_C_SOURCE=200112L")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra")

set(SRC_LIST
	time_bench.c
)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} rt)
