cmake_minimum_required(VERSION 3.5)
project(simpleargs_example LANGUAGES CXX C)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS "-lstdc++ -std=c++11 -Wall -Wfloat-conversion")
set(CMAKE_C_FLAGS "-std=c99")

# Sources
file(GLOB_RECURSE files_cpp ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE files_c ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE files_h ${PROJECT_SOURCE_DIR}/*.h)

add_executable(simpleargs_example ${files_cpp} ${files_c} ${files_h})

install(TARGETS simpleargs_example DESTINATION ${CMAKE_INSTALL_PREFIX})

set_target_properties(simpleargs_example PROPERTIES OUTPUT_NAME "simpleargs_example")

