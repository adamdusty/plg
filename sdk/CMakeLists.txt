find_package(SDL3 CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)

add_library(plg_sdk SHARED)
add_library(plg::sdk ALIAS plg_sdk)

target_link_libraries(plg_sdk PUBLIC flecs::flecs SDL3::SDL3)

target_sources(plg_sdk

    PUBLIC
        FILE_SET generated_headers
            TYPE HEADERS
            BASE_DIRS $<TARGET_PROPERTY:plg_sdk,BINARY_DIR>/export
            FILES
                ${CMAKE_CURRENT_BINARY_DIR}/export/sdk/export.hpp
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS include
            FILES
                include/plg/sdk.hpp
                include/plg/logging.hpp
    PRIVATE
        src/sdk.cpp
)

include(GenerateExportHeader)
generate_export_header(
    plg_sdk
    EXPORT_FILE_NAME "export/sdk/export.hpp"
)