find_package(Catch2 3 REQUIRED)

add_subdirectory(unit)
add_subdirectory(integration)

add_test(NAME unit COMMAND unit_tests)
add_test(NAME integration COMMAND integration_tests)