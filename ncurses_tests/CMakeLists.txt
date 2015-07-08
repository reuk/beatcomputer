cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

set(name ncurses_test)
file(GLOB sources "*.cpp")

add_executable(${name} ${sources})

set(libraries ncurses)

target_link_libraries(${name} ${libraries})
