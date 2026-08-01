# include(FetchContent)

# set(FETCHCONTENT_QUIET OFF) # Force CMake to print git clone progress

# # Save original flags
# set(OLD_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# set(OLD_CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

# # Append flag to silence ALL compiler warnings during dependency build
# if(MSVC)
#     string(APPEND CMAKE_CXX_FLAGS " /w")
#     string(APPEND CMAKE_C_FLAGS " /w")
# else()
#     string(APPEND CMAKE_CXX_FLAGS " -w")
#     string(APPEND CMAKE_C_FLAGS " -w")
# endif()


# # 1. Set the Slang version you want to target
# set(SLANG_VERSION "2026.13.1")

# # 2. Map OS and CPU architecture to Slang's asset naming format
# if(WIN32)
#     set(SLANG_OS "windows")
#     set(SLANG_EXT "zip")
# elseif(APPLE)
#     set(SLANG_OS "macos")
#     set(SLANG_EXT "zip")
# elseif(UNIX)
#     set(SLANG_OS "linux")
#     set(SLANG_EXT "tar.gz")
# else()
#     message(FATAL_ERROR "Unsupported OS for prebuilt Slang binaries.")
# endif()

# if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|amd64|AMD64)")
#     set(SLANG_ARCH "x86_64")
# elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(arm64|aarch64|ARM64)")
#     set(SLANG_ARCH "aarch64")
# else()
#     message(FATAL_ERROR "Unsupported architecture for prebuilt Slang binaries: ${CMAKE_SYSTEM_PROCESSOR}")
# endif()

# # Construct the exact release asset URL
# set(SLANG_URL "https://github.com/shader-slang/slang/releases/download/v${SLANG_VERSION}/slang-${SLANG_VERSION}-${SLANG_OS}-${SLANG_ARCH}.${SLANG_EXT}")

# message(STATUS "Fetching prebuilt Slang from: ${SLANG_URL}")

# # 3. Declare FetchContent using the dynamic URL
# FetchContent_Declare(
#     slang_prebuilt
#     URL ${SLANG_URL}
#     # DOWNLOAD_NO_EXTRACT FALSE # Extracted automatically by CMake
# )

# FetchContent_MakeAvailable(slang_prebuilt)

# # 4. Define imported target for prebuilt Slang library
# # (Since prebuilts don't export CMake targets, we create an IMPORTED target)
# add_library(slang::slang SHARED IMPORTED GLOBAL)

# set_target_properties(slang::slang PROPERTIES
#     INTERFACE_INCLUDE_DIRECTORIES "${slang_prebuilt_SOURCE_DIR}/include"
# )

# if(WIN32)
#     set_target_properties(slang::slang PROPERTIES
#         IMPORTED_IMPLIB "${slang_prebuilt_SOURCE_DIR}/lib/slang.lib"
#         IMPORTED_LOCATION "${slang_prebuilt_SOURCE_DIR}/bin/slang.dll"
#     )
# elseif(APPLE)
#     set_target_properties(slang::slang PROPERTIES
#         IMPORTED_LOCATION "${slang_prebuilt_SOURCE_DIR}/lib/libslang.dylib"
#     )
# else() # Linux
#     set_target_properties(slang::slang PROPERTIES
#         IMPORTED_LOCATION "${slang_prebuilt_SOURCE_DIR}/lib/libslang.so"
#     )
# endif()

# # Restore original flags for your project
# set(CMAKE_CXX_FLAGS "${OLD_CMAKE_CXX_FLAGS}")
# set(CMAKE_C_FLAGS "${OLD_CMAKE_C_FLAGS}")