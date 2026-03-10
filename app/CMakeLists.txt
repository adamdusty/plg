if(NOT TARGET plg_sdk)
    find_package(PLG_SDK CONFIG REQUIRED)
endif()

add_library(plg_lib OBJECT)

target_sources(plg_lib
    PUBLIC
        FILE_SET plg_lib_headers
        TYPE HEADERS
        BASE_DIRS src
        FILES
            src/lib.hpp
            src/dependency.hpp
            src/module.hpp
            src/pack.hpp
            src/version.hpp
            src/constants.hpp
    PRIVATE
        src/lib.cpp
        src/module.cpp
        src/pack.cpp
        src/version.cpp
)

target_link_libraries(plg_lib PUBLIC plg::sdk)

add_executable(plg_app)

target_sources(plg_app
    PRIVATE
        src/main.cpp
)

target_link_libraries(plg_app plg_lib)

if(WIN32)
add_custom_command(TARGET plg_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:plg_app> $<TARGET_FILE_DIR:plg_app>
    COMMAND_EXPAND_LISTS
)
endif()