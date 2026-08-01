#pragma once

#include <SDL3/SDL.h>
#include <cstring>
#include <expected>
#include <optional>
#include <string>

namespace core::rendering {

template<typename T>
auto upload_to_buffer(
    SDL_GPUDevice* dev,
    SDL_GPUCommandBuffer* cmd,
    SDL_GPUBuffer* dst,
    const T* data,
    size_t size,
    uint32_t offset = 0
) -> std::expected<void, std::string> {
    uint32_t transfer_size = static_cast<uint32_t>(size) * sizeof(T);

    auto transfer_ci = SDL_GPUTransferBufferCreateInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size  = transfer_size,
        .props = 0,
    };
    auto* transfer = SDL_CreateGPUTransferBuffer(dev, &transfer_ci);

    if(transfer == nullptr) {
        return std::unexpected(SDL_GetError());
    }

    auto* map = SDL_MapGPUTransferBuffer(dev, transfer, false);
    if(map == nullptr) {
        const auto* msg = SDL_GetError();
        SDL_ReleaseGPUTransferBuffer(dev, transfer);
        return std::unexpected(msg);
    }

    std::memcpy(map, data, transfer_size);
    SDL_UnmapGPUTransferBuffer(dev, transfer);

    auto buffer_loc = SDL_GPUTransferBufferLocation{
        .transfer_buffer = transfer,
        .offset          = 0,
    };

    auto buffer_region = SDL_GPUBufferRegion{
        .buffer = dst,
        .offset = offset,
        .size   = transfer_size,
    };

    auto* copy = SDL_BeginGPUCopyPass(cmd);
    SDL_UploadToGPUBuffer(copy, &buffer_loc, &buffer_region, false);
    SDL_EndGPUCopyPass(copy);

    SDL_ReleaseGPUTransferBuffer(dev, transfer);
    return {};
}

} // namespace core::rendering
