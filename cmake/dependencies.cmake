include(FetchContent)

if(BUILD_TESTING OR BUILD_BENCHMARKS)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY      https://github.com/catchorg/Catch2.git
    GIT_TAG             88abf9bf325c798c33f54f6b9220ef885b267f4f # v3.12.0
    GIT_SHALLOW         TRUE
    SYSTEM
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(Catch2)

endif()


FetchContent_Declare(
    flecs
    GIT_REPOSITORY      https://github.com/SanderMertens/flecs.git
    GIT_TAG             dcbaf278d15379aadea10421bbcbd3c4ead529a3 # v4.1.4
    GIT_SHALLOW         TRUE
    SYSTEM
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(flecs)

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY      https://github.com/marzer/tomlplusplus.git
    GIT_TAG             30172438cee64926dc41fdd9c11fb3ba5b2ba9de # v3.4.0
    GIT_SHALLOW         TRUE
    SYSTEM
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(tomlplusplus)
target_compile_definitions(tomlplusplus_tomlplusplus INTERFACE TOML_ENABLE_UNRELEASED_FEATURES=1)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY      https://github.com/libsdl-org/SDL.git
    GIT_TAG             a962f40bbba175e9716557a25d5d7965f134a3d3 # v3.4.0
    GIT_SHALLOW         TRUE
    SYSTEM
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    volk
    GIT_REPOSITORY      https://github.com/zeux/volk.git
    GIT_TAG             57cc6cb5bd8d5f09c5c56a6888b19043b275ec5c # master 3/9/2026
    GIT_SHALLOW         TRUE
    SYSTEM
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(volk)

FetchContent_Declare(
    VulkanMemoryAllocator
    GIT_REPOSITORY      https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG             b3cbbb43ea3a506dffe10759e205a41c27c35ae2 # master 3/9/2026
    GIT_SHALLOW         TRUE
    SYSTEM
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(VulkanMemoryAllocator)

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY      https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG             966edceaf8cdca7996c4e9a1c5ced2938de63366 # master 3/11/2026
    GIT_SHALLOW         TRUE
    SYSTEM
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(tinyobjloader)

FetchContent_Declare(
    glm
    GIT_REPOSITORY      https://github.com/g-truc/glm.git
    GIT_TAG             8d1fd52e5ab5590e2c81768ace50c72bae28f2ed # 1.0.3
    GIT_SHALLOW         TRUE
    SYSTEM
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(glm)