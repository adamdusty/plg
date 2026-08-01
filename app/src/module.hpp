#pragma once

#include <SDL3/SDL_loadso.h>
#include <expected>
#include <filesystem>
#include <flecs.h>
#include <string>
#include <unordered_map>

namespace plg {

struct module_metadata {
    std::string nspace;
    std::string name;
    std::string path;

    [[nodiscard]] auto is_valid() const -> bool {
        return !nspace.empty() && !name.empty() && !path.empty();
    }

    [[nodiscard]] constexpr auto module_id() const -> std::string { return nspace + ":" + name; }
};

struct module {
    using init   = void (*)(ecs_world_t*);
    using deinit = void (*)(ecs_world_t*);

    SDL_SharedObject* handle = nullptr;
    init initialize          = nullptr;
    deinit deinitialize      = nullptr;
};

auto read_module_metadata(const std::filesystem::path& path)
    -> std::expected<module_metadata, std::string>;
auto load_binary(const std::filesystem::path& path) -> std::expected<module, std::string>;
auto discover_modules(const std::filesystem::path& root)
    -> std::expected<std::unordered_map<std::filesystem::path, module_metadata>, std::string>;

} // namespace plg
