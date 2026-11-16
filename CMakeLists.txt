cmake_minimum_required(VERSION 3.16)
project(ip_filter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ip_filter main.cpp)

option(BUILD_TESTS "Build unit tests" OFF)

if (BUILD_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(ip_filter_tests tests_ip_filter.cpp)
    target_compile_definitions(ip_filter_tests PRIVATE UNIT_TESTING)
    target_include_directories(ip_filter_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(ip_filter_tests PRIVATE GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(ip_filter_tests)
endif()