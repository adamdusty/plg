#include "assets.hpp"

#include <SDL3/SDL.h>
#include <array>
#include <cstring>
#include <format>
#include <slang-com-ptr.h>
#include <slang.h>
#include <string>
#include <vector>

namespace core::rendering {

auto compile_shader(
    const shader_compiler_context& context,
    const std::string& name,
    const std::string& path,
    const std::string& src,
    const std::string& entry_point
) -> std::expected<std::vector<uint8_t>, std::string> {
    Slang::ComPtr<slang::IBlob> diagnostics;

    slang::IModule* shader_module = context.session->loadModuleFromSourceString(
        name.c_str(), path.c_str(), src.c_str(), diagnostics.writeRef()
    );

    if(shader_module == nullptr) {
        auto msg = std::string(
            static_cast<const char*>(diagnostics->getBufferPointer()), diagnostics->getBufferSize()
        );
        return std::unexpected(std::format("Failed to load shader\n{}", msg));
    }

    Slang::ComPtr<slang::IEntryPoint> entry = nullptr;
    if(SLANG_FAILED(shader_module->findEntryPointByName(entry_point.c_str(), entry.writeRef()))) {
        return std::unexpected(std::format("Failed to find entry point '{}'", entry_point));
    }

    auto components = std::array<slang::IComponentType*, 2>{shader_module, entry};
    Slang::ComPtr<slang::IComponentType> program;
    if(SLANG_FAILED(context.session->createCompositeComponentType(
           components.data(), 2, program.writeRef(), diagnostics.writeRef()
       ))) {
        auto msg = std::string(
            static_cast<const char*>(diagnostics->getBufferPointer()), diagnostics->getBufferSize()
        );
        return std::unexpected(std::format("Failed to create component\n{}", msg));
    }

    Slang::ComPtr<slang::IBlob> compiled_code;
    if(SLANG_FAILED(
           program->getEntryPointCode(0, 0, compiled_code.writeRef(), diagnostics.writeRef())
       )) {
        auto msg = std::string(
            static_cast<const char*>(diagnostics->getBufferPointer()), diagnostics->getBufferSize()
        );
        return std::unexpected(std::format("Failed to compile shader\n{}", msg));
    }

    auto bytecode = std::vector<uint8_t>{};
    bytecode.resize(compiled_code->getBufferSize());
    std::memcpy(bytecode.data(), compiled_code->getBufferPointer(), bytecode.size());
    return bytecode;
}

auto create_gpu_shader(
    SDL_GPUDevice* device,
    const shader_compiler_context& context,
    const std::string& name,
    const std::string& path,
    const std::string& src,
    const std::string& entry_point,
    SDL_GPUShaderStage stage
) -> std::expected<SDL_GPUShader*, std::string> {
    auto compiled = compile_shader(context, name, path, src, entry_point);
    if(!compiled.has_value()) {
        return std::unexpected(compiled.error());
    }

    auto create_info = SDL_GPUShaderCreateInfo{
        .code_size            = static_cast<Uint32>(compiled->size()),
        .code                 = compiled->data(),
        .entrypoint           = entry_point.c_str(),
        .format               = context.shader_format,
        .stage                = stage,
        .num_samplers         = 0,
        .num_storage_textures = 0,
        .num_storage_buffers  = 0,
        .num_uniform_buffers  = 2,
        .props                = 0,
    };

    auto* shader = SDL_CreateGPUShader(device, &create_info);
    if(shader == nullptr) {
        return std::unexpected(std::format("Failed to create GPU shader: {}", SDL_GetError()));
    }

    return shader;
}

auto create_gpu_shader_pair(
    SDL_GPUDevice* device,
    const shader_compiler_context& context,
    const std::string& name,
    const std::string& path,
    const std::string& src
) -> std::expected<std::pair<SDL_GPUShader*, SDL_GPUShader*>, std::string> {

    auto vert = core::rendering::create_gpu_shader(
        device, context, name, path, src, "VSMain", SDL_GPU_SHADERSTAGE_VERTEX
    );
    if(!vert) {
        return std::unexpected(std::format("Failed to compile vertex shader: {}", vert.error()));
    }

    auto frag = core::rendering::create_gpu_shader(
        device, context, name, path, src, "PSMain", SDL_GPU_SHADERSTAGE_FRAGMENT
    );

    if(!frag) {
        return std::unexpected(std::format("Failed to compile vertex shader: {}", frag.error()));
    }

    return std::pair{*vert, *frag};
}

} // namespace core::rendering
