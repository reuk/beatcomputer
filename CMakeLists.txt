cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_subdirectory(gtest)
add_subdirectory(lib)
add_subdirectory(sequencer)
add_subdirectory(time_server)

enable_testing()
add_subdirectory(tests)

include(CPack)
