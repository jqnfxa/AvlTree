cmake_minimum_required(VERSION 3.27)

project(AvlTreeProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# Enable testing
enable_testing()

# Add GoogleTest directory
find_package(GTest REQUIRED)

# Include source headers
include_directories(src/core)

# Add your source files
file(GLOB_RECURSE SOURCES "src/core/*.cpp")
add_executable(AvlTreeProject ${SOURCES})

# Add your test files
file(GLOB_RECURSE TEST_SOURCES "src/test/*.cpp")
add_executable(tests ${TEST_SOURCES})

# Link GoogleTest to the test executable
target_link_libraries(tests PRIVATE gtest GTest::GTest GTest::Main)

# Include GoogleTest's CMake functions
include(GoogleTest)

# Automatically discover tests that are defined in the test sources.
# This will result in CTest tests that mirror the Google Test tests,
# so you can use the CTest test runner to run your tests.
gtest_discover_tests(tests)

# Add a custom command that runs your tests with Valgrind
add_custom_command(OUTPUT valgrind_tests COMMAND valgrind --leak-check=full --error-exitcode=1 $<TARGET_FILE:tests> DEPENDS tests)

# Add a custom target that depends on the above command
add_custom_target(run_valgrind_tests ALL DEPENDS valgrind_tests)
