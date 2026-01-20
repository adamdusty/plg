add_library(plg_sdk SHARED)
add_library(plg::sdk ALIAS plg_sdk)

include(GenerateExportHeader)
generate_export_header(
    plg_sdk
    EXPORT_FILE_NAME "export/sdk/export.hpp"
)

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
                include/sdk/sdk.hpp
    PRIVATE
        src/sdk.cpp
)
