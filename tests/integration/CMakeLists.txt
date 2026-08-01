add_library(test_mod MODULE src/testmod.cpp)
set_target_properties(test_mod PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/integration/modules/test/mod"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/integration/modules/test/mod" # For Windows/.dll support
    PREFIX "" # Optional: removes 'lib' prefix (e.g., module.so instead of libmodule.so)
)
target_link_libraries(test_mod PRIVATE flecs::flecs)

add_executable(integration_tests
    src/app_tests.cpp
)
target_link_libraries(integration_tests PRIVATE Catch2::Catch2WithMain plg_lib plg_sdk)


if(WIN32)
add_custom_command(TARGET integration_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:integration_tests> $<TARGET_FILE_DIR:integration_tests>
    COMMAND_EXPAND_LISTS
)
endif()

add_custom_target(copy_test_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/modules"
        "$<TARGET_FILE_DIR:integration_tests>/modules"
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/packs"
        "$<TARGET_FILE_DIR:integration_tests>/packs"
)
add_dependencies(integration_tests copy_test_data)

add_test(NAME integration COMMAND integration_tests)
