set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_INIT "\
-fstack-protector-strong \
-fcf-protection=full \
-fstack-clash-protection \
-Wall \
-Wextra \
-Wpedantic \
-Wfloat-equal \
-Wconversion \
-Wsign-conversion \
-Wcast-qual \
-Wformat=2 \
-Wundef \
-Werror=float-equal \
-Wcast-align \
-Wunused \
-Wnull-dereference \
-Wdouble-promotion \
-Wimplicit-fallthrough \
-Wextra-semi \
-Woverloaded-virtual \
-Wnon-virtual-dtor \
-Wold-style-cast"
)

set(CMAKE_CXX_FLAGS_RELEASE_INIT "\
-U_FORTIFY_SOURCE \
-D_FORTIFY_SOURCE=3"
)