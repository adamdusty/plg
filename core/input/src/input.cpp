#include "input/export.hpp"
#include <SDL3/SDL.h>
#include <flecs.h>
#include <plg/sdk.hpp>

namespace {

auto register_systems(const flecs::world& wld) -> void {
    wld.system("input").run([](flecs::iter& iter) {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_EVENT_KEY_DOWN: {
                if(event.key.scancode == SDL_SCANCODE_ESCAPE) {
                    iter.world().set<plg::quit>({true});
                }
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
        SDL_Log("Initializing input plugin");
        register_systems(flecs::world{world});
    }

    CORE_INPUT_EXPORT auto deinitialize(ecs_world_t* /* world */) -> void {}
}
