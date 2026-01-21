#include "module.hpp"
#include <flecs.h>
#include <stdexcept>

constexpr auto* modules_directory_path = "modules"; // Load via config or cmd line
constexpr auto* packs_directory_path   = "packs";   // Load via config or cmd line

auto main() -> int {
    /* Initialize application
        - Should application include a window, or should that be a part of a module?
    */
    auto world = flecs::world{};

    /* Find all available modules
        - Find modules with a valid manifest
    */

    /* Find all pack defs
        - Should pack directory be hard coded?
    */

    /* Load pack modules */

    return 0;
}
