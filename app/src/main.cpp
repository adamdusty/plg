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
        throw std::runtime_error(modules.error());
    }

    /* Find all pack defs */
    auto packs = plg::find_packs(packs_directory_path);
    if(!packs) {
        throw std::runtime_error(packs.error());
    }

    /* Select pack if not supplied by config or args*/

    /* Load pack modules */

    return 0;
}
