cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

if(APPLE)
    include_directories(/usr/local/include/oscpack)
    link_directories(/usr/local/lib)
elseif(UNIX)
    include_directories(/usr/include/oscpack)
    set(libraries pthread)
endif()

include_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB sources "*.cpp")

add_executable(beatcomputer ${sources})
target_link_libraries(beatcomputer ${libraries} beatcomputer_common ncurses oscpack gflags)
