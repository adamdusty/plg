cmake_minimum_required(VERSION 4.2)

project(plg)

option(BUILD_BENCHMARKS OFF)
option(PLG_FETCH_DEPENDENCIES "Use fetch content to get dependencies" OFF)

if(PLG_FETCH_DEPENDENCIES)
include(cmake/dependencies.cmake)
endif()

add_subdirectory(sdk)
add_subdirectory(app)

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()