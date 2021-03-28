cmake_minimum_required(VERSION 3.14)
project(raytracer)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
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

FetchContent_Declare(
    argon
    GIT_REPOSITORY "https://github.com/ChrisThrasher/argon.git"
    GIT_TAG v0.7.2
)
FetchContent_MakeAvailable(argon)

add_executable(raytracer src/Raytracer.cpp)
target_include_directories(raytracer PRIVATE include)
target_link_libraries(raytracer PRIVATE argon::argon pthread)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    gtest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG master
)
FetchContent_MakeAvailable(gtest)

enable_testing()
add_executable(unit_tests test/UnitTests.cpp)
target_include_directories(unit_tests PRIVATE include)
target_include_directories(unit_tests SYSTEM PRIVATE ${gtest_SOURCE_DIR}/include)
target_link_libraries(unit_tests PRIVATE gtest)
add_test(NAME unit_tests COMMAND unit_tests)
