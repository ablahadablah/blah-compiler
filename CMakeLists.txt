cmake_minimum_required(VERSION 3.13)
project(blahCompiler)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/libfmt)
add_subdirectory(tests)

add_library(blahLib "")
target_include_directories(blahLib PUBLIC src)
target_link_libraries(blahLib fmt)
target_sources(blahLib PUBLIC
		src/lexer.cpp src/lexer.hpp src/parser/parser.cpp src/parser/parser.hpp src/parser/Expression.hpp
		src/parser/Entity.hpp src/parser/Statement.hpp src/Identifier.hpp src/translator/Translator.cpp
		src/translator/Translator.hpp)

add_executable(bppCompiler src/main.cpp)

target_include_directories(bppCompiler PUBLIC src)

target_link_libraries(bppCompiler fmt blahLib)
