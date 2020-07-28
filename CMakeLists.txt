cmake_minimum_required(VERSION 3.17)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

add_executable(main src/main.cpp)

add_subdirectory(googletest)

enable_testing()
add_executable(image_tests test/ImageTests.cpp)
target_include_directories(image_tests PRIVATE src)
target_include_directories(image_tests SYSTEM PRIVATE ${gtest_SOURCE_DIR}/include)
target_link_libraries(image_tests PRIVATE gtest)
add_test(NAME image_tests COMMAND image_tests)
