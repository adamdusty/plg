include(FetchContent)

if(BUILD_TESTING OR BUILD_BENCHMARKS)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY      https://github.com/catchorg/Catch2.git
    GIT_TAG             88abf9bf325c798c33f54f6b9220ef885b267f4f # v3.12.0
    GIT_SHALLOW         TRUE
    SYSTEM
    FIND_PACKAGE_ARGS   CONFIG
)
FetchContent_MakeAvailable(Catch2)

endif()