cmake_minimum_required(VERSION 3.24)

project(TeaBoxLang VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_definitions(-O3)

include_directories("${PROJECT_SOURCE_DIR}/include/lexer")
file(GLOB_RECURSE CPP_SOURCES "source/*.cpp")

add_executable(TeaBoxLang ${CPP_SOURCES} main.cpp)