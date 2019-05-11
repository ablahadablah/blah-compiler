cmake_minimum_required(VERSION 3.13)
project(blahCompilerTest)

set(CMAKE_CXX_STANDARD 20)


add_executable(blahCompilerTest src/main.cpp src/someTest.cpp src/lexerTest.cpp)

target_link_libraries(blahCompilerTest fmt blahLib fmt)
