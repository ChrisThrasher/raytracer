cmake_minimum_required(VERSION 3.22)
project(raytracer CXX)

if(NOT RT_EXECUTION_POLICY)
    # seq
    # par
    # par_unseq
    # unseq (c++20)
    set(RT_EXECUTION_POLICY "par")
endif()
add_definitions(-DRT_EXECUTION_POLICY=${RT_EXECUTION_POLICY})

if(NOT RT_ROW_METHOD)
    # 0 - vector
    # 1 - unique_ptr<unsigned[]>
    # 2 - counting iterator
    set(RT_ROW_METHOD 2)
endif()
add_definitions(-DRT_ROW_METHOD=${RT_ROW_METHOD})

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    find_package(TBB REQUIRED)
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast -ffast-math)
elseif(MSVC)
    add_compile_options(/WX /W4 /permissive-)
endif()

include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git)
FetchContent_MakeAvailable(SFML)

add_executable(raytracer
    src/Camera.cpp
    src/Hittable.cpp
    src/Material.cpp
    src/Random.cpp
    src/Raytracer.cpp
    src/Sphere.cpp
)

target_link_libraries(raytracer PRIVATE SFML::Graphics)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    target_link_libraries(raytracer PRIVATE TBB::tbb)
endif()

target_compile_definitions(raytracer PRIVATE FONT_PATH="${CMAKE_SOURCE_DIR}/data")

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.hpp *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy
    COMMAND run-clang-tidy -p ${CMAKE_BINARY_DIR} `git ls-files *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(run COMMAND raytracer)
