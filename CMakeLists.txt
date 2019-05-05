cmake_minimum_required(VERSION 3.13)
project(blahCompiler)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/libfmt)
add_subdirectory(tests)

add_library(blahLib "")
target_include_directories(blahLib PUBLIC src)
target_sources(blahLib PUBLIC src/lexer.cpp src/lexer.hpp src/parser/parser.cpp src/parser/parser.hpp src/parser/Expression.cpp src/parser/Expression.hpp src/parser/Entity.hpp src/parser/Statement.cpp src/parser/Statement.hpp src/Identifier.hpp src/translator/Translator.cpp src/translator/Translator.hpp)

add_executable(blahCompiler src/main.cpp)

target_include_directories(blahCompiler PUBLIC src)

target_link_libraries(blahCompiler fmt blahLib)
