#pragma once

#include "plg/logging.hpp"
#include "sdk/export.hpp"
#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

namespace plg {

PLG_SDK_EXPORT auto version() -> int;

struct PLG_SDK_EXPORT quit {
    bool should_quit = false;

    /* implicit */ operator bool() const { return should_quit; }
};

struct PLG_SDK_EXPORT window {
    SDL_Window* handle;

    /* implicit */ operator SDL_Window*() const { return handle; }
};

struct PLG_SDK_EXPORT utilities {
    spdlog::sink_ptr logging_sink = nullptr;
};

} // namespace plg
