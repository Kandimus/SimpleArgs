cmake_minimum_required(VERSION 3.5)

option(TESTS_UNIT "Enable unit tests" ON)

add_subdirectory(simpleargs)

if(${TESTS_UNIT})
    add_subdirectory(tests)
endif()
