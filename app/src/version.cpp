#include "version.hpp"

#include <charconv>
#include <regex>

namespace {

constexpr auto parse_uint(const std::string& str) -> std::expected<std::uint32_t, std::string> {
    const char* first = str.data();
    const char* last  = first + str.size(); // NOLINT

    auto value     = std::uint32_t{};
    auto [ptr, ec] = std::from_chars(first, last, value);

    if(ec == std::errc()) {
        return value;
    } else if(ec == std::errc::invalid_argument) {
        return std::unexpected("Not a number");
    } else if(ec == std::errc::result_out_of_range) {
        return std::unexpected("Number out of range");
    } else {
        return std::unexpected("Unknown error");
    }
}

} // namespace

namespace plg {

auto version::parse(const std::string& str) -> std::expected<version, std::string> {
    auto version_regex = std::regex(R"((\d+)\.(\d+)\.(\d+)(?:-([a-zA-Z0-9_\-]+))?)");

    std::smatch match;
    std::regex_match(str, match, version_regex);

    if(match.empty()) {
        return std::unexpected("Failed to parse version");
    }

    auto ver = version();
    if(auto major = parse_uint(match[1].str())) {
        ver.major = major.value();
    } else {
        return std::unexpected("Failed to parse major version");
    }
    if(auto minor = parse_uint(match[2].str())) {
        ver.minor = minor.value();
    } else {
        return std::unexpected("Failed to parse minor version");
    }
    if(auto patch = parse_uint(match[3].str())) {
        ver.patch = patch.value();
    } else {
        return std::unexpected("Failed to parse patch version");
    }

    if(match[4].matched) {
        ver.annotation = match[4].str();
    }

    return ver;
}

} // namespace plg
