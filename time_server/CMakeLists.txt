cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

if(APPLE)
    include_directories(/usr/local/include/oscpack)
    link_directories(/usr/local/lib)
elseif(UNIX)
    include_directories(/usr/include/oscpack)
endif()

set(name time_server)
file(GLOB sources "*.cpp")

set(libraries oscpack)

add_executable(${name} ${sources})
target_link_libraries(${name} ${libraries})
