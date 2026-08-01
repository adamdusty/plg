#include <flecs.h>

extern "C" {
    auto test_mod_init(ecs_world_t* _) -> void {}
    auto test_mod_deinit(ecs_world_t* _) -> void {}
}
