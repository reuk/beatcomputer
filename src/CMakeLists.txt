cmake_minimum_required(VERSION 3.0)

#find_program(LEX_EXE flex)
#macro(lex_file _filename)
#    get_filename_component(_basename ${_filename} NAME_WE)
#    add_custom_command(
#        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.c
#        COMMAND ${LEX_EXE} -o${_basename}.c ${CMAKE_CURRENT_SOURCE_DIR}/${_filename}
#        DEPENDS ${_filename})
#endmacro(lex_file)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++1y")

set(name beatcomputer)
file(GLOB sources "*.cpp")

add_executable(${name} ${sources})

set(libraries ncurses)

target_link_libraries(${name} ${libraries})

#lex_file(lex.l)
#add_executable(lexer lex.c)
#target_link_libraries(lexer l)
