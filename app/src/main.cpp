#include "module.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <algorithm>
#include <filesystem>
#include <flecs.h>
#include <plg/sdk.hpp>
#include <ranges>
#include <vector>

#if __linux__
constexpr auto input_name     = "core_input.so";
constexpr auto rendering_name = "core_rendering.so";
#else
constexpr auto input_name     = "core_input.dll";
constexpr auto rendering_name = "core_rendering.dll";
#endif

auto main(int /*argc*/, char* /*argv*/[]) -> int {
    auto base_path = std::filesystem::path(SDL_GetBasePath());

    /* Initialize application */
    auto world = flecs::world{};
    flecs::log::set_level(-1);
    world.set<plg::quit>({false});

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Vulkan_LoadLibrary(nullptr);
    auto window = plg::window(SDL_CreateWindow("plg", 640, 480, SDL_WINDOW_VULKAN));
    world.set<plg::window>(window);

    auto mod_paths = std::vector<std::filesystem::path>();
    mod_paths.emplace_back(base_path / "mods" / input_name);
    mod_paths.emplace_back(base_path / "mods" / rendering_name);

    SDL_Log("loading binaries");
    auto mods = mod_paths
              | std::ranges::views::transform([](auto& path) { return plg::load_binary(path); })
              | std::ranges::views::filter([](auto b) { return b.has_value(); })
              | std::ranges::views::transform([](auto b) { return *b; })
              | std::ranges::to<std::vector>();

    SDL_Log("Loaded %lu binaries", mods.size());

    SDL_Log("initializing");
    std::ranges::for_each(mods, [&](auto mod) {
        if(mod.initialize != nullptr) {
            mod.initialize(world);
        }
    });

    SDL_Log("progress");
    // while(!world.get<plg::quit>()) {
    //     world.progress();
    // }

    SDL_DestroyWindow(window);

    return 0;
}
