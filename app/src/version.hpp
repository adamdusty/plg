#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <string>

namespace plg {

struct version {
    std::uint32_t major{};
    std::uint32_t minor{};
    std::uint32_t patch{};
    std::optional<std::string> annotation;

    version() = default;
    version(std::uint32_t major, std::uint32_t minor, std::uint32_t patch) :
        major(major), minor(minor), patch(patch) {}
    version(std::uint32_t major, std::uint32_t minor, std::uint32_t patch, std::string ann) :
        major(major), minor(minor), patch(patch), annotation(ann) {}

    auto operator==(const version& other) const -> bool {
        return major == other.major && minor == other.minor && patch == other.patch;
    }

    static auto parse(const std::string& str) -> std::expected<version, std::string>;
};

} // namespace plg
