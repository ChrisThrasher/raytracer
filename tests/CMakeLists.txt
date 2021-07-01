set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(gtest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG master)
FetchContent_MakeAvailable(gtest)

add_executable(unit_tests UnitTests.cpp)
target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(unit_tests PRIVATE gtest)
add_test(NAME unit_tests COMMAND unit_tests)
