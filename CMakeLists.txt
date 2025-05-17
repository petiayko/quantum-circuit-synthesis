cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static")

option(BUILD_TESTS "Build tests" ON)
option(BUILD_COVERAGE "Build code coverage" ON)

include("cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.317.tar.gz"
        SHA1 "fbdd94b1966d351384e27b02c8d134915b1131d6"
)

project(quantum_circuit_synthesis VERSION 0.1.0)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/gates.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/primitives.cpp
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
    add_executable(tests
            ${CMAKE_CURRENT_SOURCE_DIR}
            tests/test_boolean_functions.cpp
            tests/test_mappings.cpp
            tests/test_substitutions.cpp
            tests/test_gates.cpp
            tests/test_circuits.cpp

            tests/test_math.cpp
            tests/test_strings.cpp
    )

    target_link_libraries(tests ${PROJECT_NAME} GTest::gtest_main)
    enable_testing()
    add_test(NAME unit_tests COMMAND tests)
endif ()
