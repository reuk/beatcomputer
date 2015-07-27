cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

if(APPLE)
    include_directories(/usr/local/include/oscpack)
elseif(UNIX)
    include_directories(/usr/include/oscpack)
endif()

set(name beatcomputer)
file(GLOB lib_sources "*.cpp")

add_library(${name}_common ${lib_sources})
target_link_libraries(${name}_common oscpack)

set(libraries ncurses oscpack)
add_executable(${name} main.cpp)

target_link_libraries(${name} ${name}_common ${libraries})
