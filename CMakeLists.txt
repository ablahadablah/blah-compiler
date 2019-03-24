cmake_minimum_required(VERSION 3.13)
project(blahCompiler)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/libfmt)
add_subdirectory(tests)

add_executable(blahCompiler main.cpp lexer.cpp lexer.hpp tests/src/main.cpp)

target_link_libraries(blahCompiler fmt)
