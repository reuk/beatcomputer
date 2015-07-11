cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

set(name beatcomputer)
file(GLOB sources "*.cpp")

set(libraries ncurses)
add_executable(${name} ${sources})
target_link_libraries(${name} ${libraries})
