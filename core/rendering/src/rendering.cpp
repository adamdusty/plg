#include "rendering/export.hpp"

#include "initialization.hpp"
#include <SDL3/SDL.h>
#include <flecs.h>
#include <volk.h>

extern "C" {
    CORE_RENDERING_EXPORT auto initialize(ecs_world_t* world) -> void {
        SDL_Log("Initializing core rendering module");

        volkInitialize();

        SDL_Log("Creating instance");
        auto* instance = core::rendering::create_instance();
        if(instance != nullptr) {
            SDL_Log("Created instance successfully");
        } else {
            SDL_Log("Failed to create instance");
        }

        volkLoadInstance(instance);
    }

    CORE_RENDERING_EXPORT auto deinitialize(ecs_world_t* _) -> void {}
}
