#include "module.hpp"
#include <flecs.h>

constexpr auto* modules_directory_path = "modules"; // Load via config or cmd line
constexpr auto* packs_directory_path   = "packs";   // Load via config or cmd line

auto main() -> int {
    /* Initialize application
        - Should application include a window, or should that be a part of a module?
    */
    auto world = flecs::world{};

    /* Find all available modules
        - Should the modules directory be hard coded?
    */
    auto modules = plg::find_available_modules(modules_directory_path);

    /* Find all pack defs
        - Should pack directory be hard coded?
    */

    return 0;
}
