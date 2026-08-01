#include "module.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <algorithm>
#include <filesystem>
#include <flecs.h>
#include <map>
#include <plg/sdk.hpp>
#include <plg/vfs.hpp>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <vector>

#if SDL_PLATFORM_WIN32
constexpr auto system_shared_lib_ext = ".dll";
#elif SDL_PLATFORM_LINUX
constexpr auto system_shared_lib_ext = ".so";
#elif SDL_PLATFORM_MACOS
constexpr auto system_shared_lib_ext = ".dynlib";
#endif

auto main(int /*argc*/, char* /*argv*/[]) -> int {
    auto base_path = std::filesystem::path(SDL_GetBasePath());

    spdlog::init_thread_pool(512, 1);
    spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto logger           = std::make_shared<spdlog::async_logger>(
        "engine", sink, spdlog::thread_pool(), spdlog::async_overflow_policy::block
    );
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);

    auto vfs = plg::virtual_file_system{};

    /* Initialize application */
    auto world = flecs::world{};
    flecs::log::set_level(-1);
    world.set<plg::quit>({false});
    world.set<plg::utilities>({sink});

    SDL_Init(SDL_INIT_VIDEO);
    auto window = plg::window(SDL_CreateWindow("plg", 640, 480, SDL_WINDOW_RESIZABLE));
    world.set<plg::window>(window);

    SPDLOG_INFO("Searching for modules");
    auto mod_paths = plg::discover_modules(base_path / "mods");
    if(!mod_paths) {
        SPDLOG_ERROR("Error while searching for modules: {}", mod_paths.error());
        return 1;
    } else {
        SPDLOG_INFO("Found {} modules", mod_paths->size());
    }

    auto mods = std::vector<std::tuple<fs::path, plg::module_metadata, plg::module>>();
    for(const auto& [mod_dir, metadata]: *mod_paths) {
        auto path = mod_dir / std::format("{}{}", metadata.path, system_shared_lib_ext);
        if(auto res = plg::load_binary(path)) {
            mods.emplace_back(mod_dir, metadata, *res);
        } else {
            SPDLOG_ERROR(res.error());
        }
    }

    SPDLOG_INFO("Loaded {} binaries", mods.size());

    SPDLOG_INFO("Initializing modules");
    for(const auto& [dir, met, mod]: mods) {
        vfs.mount(met.module_id(), dir);
        if(mod.initialize != nullptr) {
            mod.initialize(world);
        }

        SPDLOG_INFO("Initialized module {}", met.module_id());
    };

    world.import<flecs::stats>();
    world.set<flecs::Rest>({});

    SPDLOG_INFO("Starting systems");
    while(!world.get<plg::quit>()) {
        world.progress();
    }

    SPDLOG_INFO("Deinitializing modules");
    for(const auto& [dir, met, mod]: mods) {
        if(mod.deinitialize != nullptr) {
            mod.deinitialize(world);
        }

        vfs.unmount_module(met.module_id());
    };

    SDL_DestroyWindow(window);
    spdlog::shutdown();

    return 0;
}
