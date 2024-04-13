cmake_minimum_required(VERSION 3.20.4)
project(Tasks C)
set(CMAKE_C_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(client)
add_subdirectory(server)
