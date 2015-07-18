cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

include_directories(/usr/local/include/oscpack)

set(name time_server)
file(GLOB sources "*.cpp")

set(libraries oscpack)

add_executable(${name} ${sources})
target_link_libraries(${name} ${libraries})
