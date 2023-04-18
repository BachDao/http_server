cmake_minimum_required(VERSION 3.11)

## dependencies is managed by vcpkg
include(FetchContent)
FetchContent_Declare(vcpkg
        GIT_REPOSITORY https://github.com/microsoft/vcpkg/
        GIT_TAG 2023.02.24
        )
FetchContent_MakeAvailable(vcpkg)
set(CMAKE_TOOLCHAIN_FILE "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")


project(http_server VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(http_server ${SRC})
target_include_directories(http_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


find_package(llhttp CONFIG REQUIRED)
target_link_libraries(http_server PUBLIC llhttp::llhttp_static)

add_subdirectory(examples)
add_subdirectory(test)