find_package(Catch2 3 REQUIRED)

add_subdirectory(unit)

add_test(NAME unit COMMAND unit_tests)