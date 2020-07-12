cmake_minimum_required(VERSION 3.17)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_BUILD_TYPE Release)

add_compile_options(
    -Werror
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(main main.cpp)
