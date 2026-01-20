find_package(flecs CONFIG REQUIRED)
find_package(simdjson CONFIG REQUIRED)

add_library(plg_lib OBJECT)

target_sources(plg_lib
    PUBLIC
        FILE_SET plg_lib_headers
        TYPE HEADERS
        BASE_DIRS src
        FILES
            src/lib.hpp
            src/module.hpp
    PRIVATE
        src/lib.cpp
        src/module.cpp
)

target_link_libraries(plg_lib PUBLIC flecs::flecs simdjson)

add_executable(plg_app)

target_sources(plg_app
    PRIVATE
        src/main.cpp
)

target_link_libraries(plg_app plg_lib)