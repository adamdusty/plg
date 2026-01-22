#pragma once

#include <compare>
#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <tuple>

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

    static auto parse(const std::string& str) -> std::expected<version, std::string>;

    constexpr auto operator==(const version& other) const -> bool {
        return std::tie(major, minor, patch) == std::tie(other.major, other.minor, other.patch);
    }

    constexpr auto operator<=>(const version& other) const -> std::strong_ordering {
        return std::tie(major, minor, patch) <=> std::tie(other.major, other.minor, other.patch);
    }
};

} // namespace plg
