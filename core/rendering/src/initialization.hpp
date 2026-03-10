#pragma once

#include <volk.h>
#include <vulkan/vulkan.h>

namespace core::rendering {

auto create_instance() -> VkInstance;

} // namespace core::rendering
