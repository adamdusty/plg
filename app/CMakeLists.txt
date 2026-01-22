find_package(flecs CONFIG REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)

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
            src/parsing.hpp
    PRIVATE
        src/lib.cpp
        src/module.cpp
        src/pack.cpp
        src/version.cpp
        src/parsing.cpp
)

target_link_libraries(plg_lib PUBLIC flecs::flecs tomlplusplus::tomlplusplus)

add_executable(plg_app)

target_sources(plg_app
    PRIVATE
        src/main.cpp
)

target_link_libraries(plg_app plg_lib)