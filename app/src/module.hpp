#pragma once

#include <SDL3/SDL_loadso.h>
#include <expected>
#include <filesystem>
#include <flecs.h>
#include <string>

namespace plg {

struct module {
    using init   = void (*)(ecs_world_t*);
    using deinit = void (*)(ecs_world_t*);

    SDL_SharedObject* handle = nullptr;
    init initialize          = nullptr;
    deinit deinitialize      = nullptr;
};

auto load_binary(const std::filesystem::path& path) -> std::expected<module, std::string>;

} // namespace plg
