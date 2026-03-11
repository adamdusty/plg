#include "module.hpp"

#include <SDL3/SDL.h>

namespace plg {

auto load_binary(const std::filesystem::path& path) -> std::expected<module, std::string> {
    auto* obj = SDL_LoadObject(path.string().c_str());
    if(obj == nullptr) {
        SDL_Log("%s", SDL_GetError());
        return std::unexpected("Null");
    }

    auto init   = reinterpret_cast<module::init>(SDL_LoadFunction(obj, "initialize"));
    auto deinit = reinterpret_cast<module::deinit>(SDL_LoadFunction(obj, "deinitialize"));

    return module{obj, init, deinit};
}

} // namespace plg
