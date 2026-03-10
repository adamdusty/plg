if(NOT TARGET plg_sdk)
    find_package(PLG_SDK CONFIG REQUIRED)
endif()

add_library(core_input MODULE)
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

target_link_libraries(core_input PUBLIC plg::sdk)

include(GenerateExportHeader)
generate_export_header(
    core_input
    EXPORT_FILE_NAME "export/input/export.hpp"
)