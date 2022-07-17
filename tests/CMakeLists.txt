cmake_minimum_required(VERSION 3.5)
project(simpleargs_test LANGUAGES CXX)

set(CXX_STANDARD 11)

add_executable(${PROJECT_NAME} "example.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE simpleargs)
