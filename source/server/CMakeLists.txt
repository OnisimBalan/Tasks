cmake_minimum_required(VERSION 3.20.4)
project(Tasks_Client C)
set(CMAKE_C_STANDARD 17)

file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/public/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")

add_executable(tasks_server ${SRC_FILES})
set_target_properties(tasks_server PROPERTIES LINKER_LANGUAGE C)
target_include_directories(tasks_server PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public")
target_compile_options(tasks_server PUBLIC -lpthread)
