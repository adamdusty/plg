#include "rendering.hpp"

#include "components.hpp"
#include "rendering/export.hpp"
#include "systems.hpp"
#include <flecs.h>
#include <plg/sdk.hpp>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/spdlog.h>

extern "C" {
    CORE_RENDERING_EXPORT auto initialize(ecs_world_t* wld) -> void {
        auto world         = flecs::world{wld};
        const auto& window = world.get<plg::window>();
        const auto& utils  = world.get<plg::utilities>();

        auto logger = std::make_shared<spdlog::async_logger>(
            "core:rendering",
            utils.logging_sink,
            spdlog::thread_pool(),
            spdlog::async_overflow_policy::block
        );

        SPDLOG_LOGGER_INFO(logger, "Initializing core rendering module");

        auto app_res = core::rendering::create_application_resources(window);
        if(!app_res) {
            SPDLOG_LOGGER_ERROR(
                logger, "Failed to create application resources: {}", SDL_GetError()
            );
        }

        app_res->logger = logger;

        SPDLOG_LOGGER_INFO(logger, "Create application resources");
        world.set<core::rendering::application_resources>(*app_res);

        auto e = world.entity();
        e.add<core::rendering::transform>();
        e.add<core::rendering::mesh_geometry>();
        e.add<core::rendering::material_data>();

        // auto compiler_res = core::rendering::create_shader_compiler_context(context.device);
        // if(!compiler_res) {
        //     SDL_Log("%s", compiler_res.error().c_str());
        // }

        // auto res = core::rendering::materials::phong(context, *compiler_res);
        // if(!res) {
        //     SDL_Log("%s", res.error().c_str());
        // }

        // SDL_Log("Renderer init");

        // world
        //     .system<
        //         core::rendering::mesh_geometry,
        //         core::rendering::material_data,
        //         core::rendering::transform>("generate_draw_commands")
        //     .kind(flecs::OnValidate)
        //     .run(core::rendering::generate_draw_commands);
        // world.system("render_scene").kind(flecs::PostUpdate).run(core::rendering::render_scene);
    }

    CORE_RENDERING_EXPORT auto deinitialize(ecs_world_t* wld) -> void {
        auto world      = flecs::world{wld};
        const auto& res = world.get<core::rendering::application_resources>();

        SDL_ReleaseWindowFromGPUDevice(res.device, res.window);
        SDL_DestroyGPUDevice(res.device);
    }
}

namespace core::rendering {

auto create_application_resources(SDL_Window* win, bool debug, const char* driver)
    -> std::expected<application_resources, std::string> {
    auto shader_fmts = SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_SPIRV
                     | SDL_GPU_SHADERFORMAT_MSL;
    auto* dev = SDL_CreateGPUDevice(shader_fmts, debug, driver);
    if(dev == nullptr) {
        return std::unexpected("Failed to create device");
    }

    if(!SDL_ClaimWindowForGPUDevice(dev, win)) {
        return std::unexpected("Failed to claim window for gpu device");
    }

    return application_resources{.window = win, .device = dev};
}

} // namespace core::rendering
