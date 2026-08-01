add_library(core_rendering MODULE)
target_link_libraries(core_rendering plg::sdk)
# target_compile_definitions(core_rendering PRIVATE VK_NO_PROTOTYPES)
target_sources(core_rendering
    PUBLIC
        FILE_SET generated_headers
            TYPE HEADERS
            BASE_DIRS $<TARGET_PROPERTY:core_rendering,BINARY_DIR>/export
            FILES
                ${CMAKE_CURRENT_BINARY_DIR}/export/rendering/export.hpp
    PRIVATE
        FILE_SET private_headers
            TYPE HEADERS
            BASE_DIRS src
            FILES
                src/rendering.hpp
                src/systems.hpp
                src/components.hpp
                src/configuration.hpp
                src/allocation.hpp
                src/gpu.hpp
    PRIVATE
        src/rendering.cpp
        src/systems.cpp
        src/gpu.cpp
        src/allocation.cpp
)


set_target_properties(core_rendering PROPERTIES
    OUTPUT_NAME "rendering"
    PREFIX  ""
)


include(GenerateExportHeader)
generate_export_header(
    core_rendering
    EXPORT_FILE_NAME "export/rendering/export.hpp"
)

add_custom_command(
    TARGET core_rendering
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}/mods/core/rendering"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:core_rendering>" "${PACKAGE_DIR}/mods/core/rendering"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/module.json" "${PACKAGE_DIR}/mods/core/rendering"
)