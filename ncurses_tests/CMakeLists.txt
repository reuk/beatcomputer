cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

set(libraries ncurses)

set(exes window_test windowing)

foreach(i ${exes})
    add_executable(${i} ${i}.cpp)
    target_link_libraries(${i} ${libraries})
endforeach(i)
