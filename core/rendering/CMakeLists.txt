if(NOT TARGET plg_sdk)
    find_package(PLG_SDK CONFIG REQUIRED)
endif()

find_package(Vulkan REQUIRED)
find_package(volk CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

add_library(core_rendering MODULE)
target_link_libraries(core_rendering plg::sdk volk GPUOpen::VulkanMemoryAllocator tinyobjloader)
target_compile_definitions(core_rendering PRIVATE VK_NO_PROTOTYPES)
target_sources(core_rendering
    PUBLIC
        FILE_SET generated_headers
            TYPE HEADERS
            BASE_DIRS $<TARGET_PROPERTY:core_rendering,BINARY_DIR>/export
            FILES
                ${CMAKE_CURRENT_BINARY_DIR}/export/rendering/export.hpp
    PRIVATE
        src/components.hpp
        src/initialization.hpp
        src/initialization.cpp
        src/assets.hpp
        src/assets.cpp
        src/rendering.cpp
        src/vma.cpp
)


include(GenerateExportHeader)
generate_export_header(
    core_rendering
    EXPORT_FILE_NAME "export/rendering/export.hpp"
)

