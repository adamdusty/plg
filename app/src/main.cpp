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
        - Should the modules directory be hard coded?
    */
    auto modules = plg::find_modules(modules_directory_path);
    if(!modules.has_value()) {
        throw std::runtime_error(modules.error());
    }

    // Remove modules that returned error
    modules->erase(
        std::remove_if(
            modules->begin(), modules->end(), [](const auto& res) { return !res.has_value(); }
        ),
        modules->end()
    );

    /* Find all pack defs
        - Should pack directory be hard coded?
    */

    /* Load pack modules */

    return 0;
}
