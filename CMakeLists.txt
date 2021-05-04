cmake_minimum_required(VERSION 3.14)
project(raytracer CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FetchContent_Declare(argon
    GIT_REPOSITORY "https://github.com/ChrisThrasher/argon.git"
    GIT_TAG v0.7.2)
FetchContent_MakeAvailable(argon)

add_compile_options(
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wold-style-cast
)

add_executable(raytracer src/Raytracer.cpp)
target_include_directories(raytracer PRIVATE include)
target_link_libraries(raytracer PRIVATE argon::argon pthread)

enable_testing()
add_subdirectory(test)
