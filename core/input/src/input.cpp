#include "input/export.hpp"
#include <SDL3/SDL.h>
#include <flecs.h>
#include <plg/sdk.hpp>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/spdlog.h>

namespace {

auto register_systems(const flecs::world& wld) -> void {
    wld.system("input").kind(flecs::PreUpdate).run([](flecs::iter& iter) {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_EVENT_KEY_DOWN: {
                if(event.key.scancode == SDL_SCANCODE_ESCAPE) {
                    iter.world().set<plg::quit>({true});
                }
                break;
            }
            case SDL_EVENT_QUIT: {
                iter.world().set<plg::quit>({true});
                break;
            }
            default:
                break;
            }
        }
    });
}

} // namespace

extern "C" {
    CORE_INPUT_EXPORT auto initialize(ecs_world_t* world) -> void {
        auto wld   = flecs::world{world};
        auto utils = wld.get<plg::utilities>();

        auto logger = std::make_shared<spdlog::async_logger>(
            "core:input",
            utils.logging_sink,
            spdlog::thread_pool(),
            spdlog::async_overflow_policy::block
        );
        SPDLOG_LOGGER_INFO(logger, "Initializing input plugin");
        register_systems(flecs::world{world});
    }

    CORE_INPUT_EXPORT auto deinitialize(ecs_world_t* /* world */) -> void {}
}
