cmake_minimum_required(VERSION 3.0)

project(tests)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

include_directories(
    ${CMAKE_SOURCE_DIR}/sequencer
    ${CMAKE_SOURCE_DIR}/gtest/include
)

set(name tests)
file(GLOB sources "*.cpp")

add_definitions(${test_file_flag})

add_executable(${name} ${sources})

target_link_libraries(${name} beatcomputer_common gtest)
