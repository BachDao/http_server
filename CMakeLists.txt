cmake_minimum_required(VERSION 3.10)
project(http_server VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(http_server ${SRC})
target_include_directories(http_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(examples)