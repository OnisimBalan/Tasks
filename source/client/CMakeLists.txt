cmake_minimum_required(VERSION 3.20.4)
project(Tasks_Server C)
set(CMAKE_C_STANDARD 17)

file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/public/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")

add_executable(tasks_client ${SRC_FILES})
set_target_properties(tasks_client PROPERTIES LINKER_LANGUAGE C)
target_include_directories(tasks_client PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/public")
target_compile_options(tasks_client PUBLIC -lpthread)
