find_package(Catch2 3 REQUIRED)

add_executable(benchmarks)
target_sources(benchmarks
    PRIVATE
        src/app.cpp
)

target_link_libraries(benchmarks Catch2::Catch2WithMain plg_lib)

add_test(NAME benchmarks COMMAND benchmarks)