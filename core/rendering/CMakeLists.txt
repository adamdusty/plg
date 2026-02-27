add_library(core_rendering MODULE)
target_sources(core_rendering
    PRIVATE
        src/rendering.cpp
)

add_custom_command(TARGET core_rendering POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/manifest.toml"
        "$<TARGET_FILE_DIR:core_rendering>"
)