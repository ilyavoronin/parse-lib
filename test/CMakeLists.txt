cmake_minimum_required(VERSION 3.16)
project(parse_lib_tests)

set(CMAKE_CXX_STANDARD 17)

add_executable(parse_lib_tests ParserTests.cpp
        ../src/BasicParsers.cpp ../src/SStream.cpp ParserCombinationTests.cpp)

target_link_libraries(parse_lib_tests gtest gtest_main)