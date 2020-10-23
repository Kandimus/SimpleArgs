cmake_minimum_required(VERSION 3.5)
project(simpleargs_example LANGUAGES CXX C)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS "-lstdc++ -std=c++11 -Wall -Wfloat-conversion")
set(CMAKE_C_FLAGS "-std=c99")

# Sources
file(GLOB_RECURSE files_cpp ${PROJECT_SOURCE_DIR}/*.cpp
                            ${PROJECT_SOURCE_DIR}/*.c
                            ${PROJECT_SOURCE_DIR}/example/*.cpp
                            ${PROJECT_SOURCE_DIR}/example/*.c)
file(GLOB_RECURSE files_h   ${PROJECT_SOURCE_DIR}/*.h
                            ${PROJECT_SOURCE_DIR}/example/*.h)

add_executable(simpleargs_example ${files_cpp} ${files_h})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/example")

install(TARGETS simpleargs_example DESTINATION ${CMAKE_INSTALL_PREFIX})

set_target_properties(simpleargs_example PROPERTIES OUTPUT_NAME "simpleargs_example")

