if(NOT TARGET plg_sdk)
    find_package(PLG_SDK CONFIG REQUIRED)
endif()

add_library(core_input MODULE)
target_link_libraries(core_input PUBLIC plg::sdk)
target_sources(core_input
    PUBLIC
        FILE_SET generated_headers
            TYPE HEADERS
            BASE_DIRS $<TARGET_PROPERTY:core_input,BINARY_DIR>/export
            FILES
                ${CMAKE_CURRENT_BINARY_DIR}/export/input/export.hpp
    PRIVATE
        src/input.cpp
)

set_target_properties(core_input PROPERTIES
    OUTPUT_NAME "input"
    PREFIX  ""
)

include(GenerateExportHeader)
generate_export_header(
    core_input
    EXPORT_FILE_NAME "export/input/export.hpp"
)

add_custom_command(
    TARGET core_input
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}/mods/core/input"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:core_input>" "${PACKAGE_DIR}/mods/core/input"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/module.json" "${PACKAGE_DIR}/mods/core/input"
)