#include "module.hpp"
#include "pack.hpp"
#include <flecs.h>

constexpr auto* modules_directory_path = "modules"; // Load via config or cmd line
constexpr auto* packs_directory_path   = "packs";   // Load via config or cmd line

auto main(int /*argc*/, char* /*argv*/[]) -> int {
    /* Initialize application */
    auto world = flecs::world{};

    /* Find all available modules */
    auto modules = plg::find_modules(modules_directory_path);
    if(!modules) {
        return -1;
    }

    /* Find all pack defs */
    auto packs = plg::find_packs(packs_directory_path);
    if(!packs) {
        return -1;
    }

    /* Select pack if not supplied by config or args*/
    auto pack = packs->front();

    /* Load pack modules */
    for(const auto& mod: pack.dependencies) {}

    return 0;
}
