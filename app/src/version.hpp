#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace plg {

struct version {
    std::uint32_t major{};
    std::uint32_t minor{};
    std::uint32_t patch{};
    std::optional<std::string> annotation;

    version(std::uint32_t major, std::uint32_t minor, std::uint32_t patch) :
        major(major), minor(minor), patch(patch) {}

    auto operator==(const version& other) const -> bool {
        return major == other.major && minor == other.minor && patch == other.patch;
    }
};

} // namespace plg
