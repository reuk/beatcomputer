cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

if(APPLE)
    include_directories(/usr/local/include/oscpack)
    link_directories(/usr/local/lib)
elseif(UNIX)
    include_directories(/usr/include/oscpack)
endif()

file(GLOB sources "*.cpp")

add_library(beatcomputer_common ${sources})
target_link_libraries(beatcomputer_common oscpack)
