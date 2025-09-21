cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(VERSION "1.0.1")

option(BUILD_TESTS "Build tests" ON)
option(BUILD_COVERAGE "Build code coverage" ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

project(quantum_circuit_synthesis VERSION ${VERSION})
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_library(${PROJECT_NAME} STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/gates.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/primitives.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/synthesis.cpp
)

add_executable(qcs
        ${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include>"
)

target_link_libraries(qcs ${PROJECT_NAME})

if (BUILD_TESTS)
    # https://google.github.io/googletest/quickstart-cmake.html
    enable_testing()
    add_executable(tests
            ${CMAKE_CURRENT_SOURCE_DIR}
            tests/test_boolean_functions.cpp
            tests/test_mappings.cpp
            tests/test_substitutions.cpp
            tests/test_gates.cpp
            tests/test_circuits.cpp
            tests/test_synthesis.cpp

            tests/test_math.cpp
            tests/test_strings.cpp
    )

    target_link_libraries(tests ${PROJECT_NAME} GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(tests)
endif ()
