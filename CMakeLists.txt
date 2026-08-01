cmake_minimum_required(VERSION 4.0)

project(plg)

option(BUILD_BENCHMARKS OFF)
option(PLG_FETCH_DEPENDENCIES "Use fetch content to get dependencies" OFF)

if(PLG_FETCH_DEPENDENCIES)
include(cmake/dependencies.cmake)
endif()

include(cmake/package.cmake)

if(BUILD_TESTING OR BUILD_BENCHMARKS)
add_subdirectory(vendor/catch2 SYSTEM)
endif()

set(SPDLOG_BUILD_SHARED ON)
set(SPDLOG_BUILD_PIC ON)
add_subdirectory(vendor/spdlog SYSTEM)
add_subdirectory(vendor/flecs SYSTEM)
add_subdirectory(vendor/sdl3 SYSTEM)
add_subdirectory(vendor/glm SYSTEM)
add_subdirectory(vendor/glaze SYSTEM)

add_subdirectory(sdk)
add_subdirectory(app)
add_subdirectory(core)

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
