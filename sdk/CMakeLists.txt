add_library(plg_sdk SHARED)
add_library(plg::sdk ALIAS plg_sdk)

target_sources(plg_sdk
    PUBLIC
        FILE_SET headers
        TYPE HEADERS
        BASE_DIRS include
        FILES
            include/sdk/sdk.hpp
    PRIVATE
        src/sdk.cpp
)