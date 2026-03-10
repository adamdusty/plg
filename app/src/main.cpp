#include "module.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <algorithm>
#include <filesystem>
#include <flecs.h>
#include <plg/sdk.hpp>
#include <ranges>

auto log(const char* msg) -> void {
    ecs_log(-1, msg);
}

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
    mod_paths.emplace_back(base_path / "mods" / "core_rendering.dll");
    mod_paths.emplace_back(base_path / "mods" / "core_input.dll");

    log("loading binary");
    auto mods = mod_paths
              | std::ranges::views::transform([](auto& path) { return plg::load_binary(path); })
              | std::ranges::views::filter([](auto b) { return b.has_value(); })
              | std::ranges::views::transform([](auto b) { return *b; })
              | std::ranges::to<std::vector>();

    log("initializing");
    std::ranges::for_each(mods, [&](auto mod) {
        if(mod.initialize != nullptr) {
            mod.initialize(world);
        }
    });

    log("progress");
    while(!world.get<plg::quit>()) {
        world.progress();
    }

    SDL_DestroyWindow(window);

    return 0;
}
