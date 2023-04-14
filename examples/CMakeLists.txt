

file(GLOB EXAMPLES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

link_libraries(http_server)

add_executable(hello_world hello_world.cpp)
