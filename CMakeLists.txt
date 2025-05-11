cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_COVERAGE "Build code coverage" ON)

#set(
#        HUNTER_CACHE_SERVERS
#        "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
#        CACHE STRING "Default cache server"
#)

include("cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.314.tar.gz"
        SHA1 "95c47c92f68edb091b5d6d18924baabe02a6962a"
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
            tests/test_primitives.cpp
            tests/test_mappings.cpp
            tests/test_substitutions.cpp
    )

    target_link_libraries(tests ${PROJECT_NAME} GTest::gtest_main)
    enable_testing()
    add_test(NAME unit_tests COMMAND tests)
endif ()
