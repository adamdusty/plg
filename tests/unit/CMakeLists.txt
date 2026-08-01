add_executable(unit_tests
    src/app_tests.cpp
    src/sdk_tests.cpp
)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain plg_lib plg_sdk)


if(WIN32)
add_custom_command(TARGET unit_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:unit_tests> $<TARGET_FILE_DIR:unit_tests>
    COMMAND_EXPAND_LISTS
)
endif()

add_test(NAME unit COMMAND unit_tests)