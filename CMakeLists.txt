cmake_minimum_required(VERSION 3.13)
project(blahCompiler)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/libfmt)
add_subdirectory(tests)

add_executable(blahCompiler src/main.cpp src/lexer.cpp src/lexer.hpp src/parser/parser.cpp src/parser/parser.hpp src/parser/Expression.cpp src/parser/Expression.hpp src/parser/Entity.hpp src/parser/Statement.cpp src/parser/Statement.hpp src/Identifier.hpp)

target_include_directories(blahCompiler PUBLIC src)

target_link_libraries(blahCompiler fmt)
