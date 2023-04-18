enable_testing()

file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(test ${TEST_FILES})

find_package(GTest CONFIG REQUIRED)
target_link_libraries(test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_link_libraries(test PRIVATE http_server)