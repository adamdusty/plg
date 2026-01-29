#include "pack.hpp"

#include "parsing.hpp"
#include <filesystem>
#include <fstream>
#include <ranges>
#include <toml++/toml.hpp>

namespace fs = std::filesystem;

namespace {
auto pack_from_direntry(const fs::directory_entry& entry) -> std::optional<plg::pack> {
    auto file = std::ifstream(entry.path());
    if(!file) {
        return std::nullopt;
    }

    toml::table table;

    try {
        table = toml::parse(file);
    } catch(const toml::parse_error& e) {
        return std::nullopt;
    }

    auto pack = plg::parsing::from_toml<plg::pack>(table);
    if(!pack.has_value()) {
        return std::nullopt;
    }

    return *pack;
}
} // namespace

namespace plg {

auto find_packs(const std::string& dir) -> std::expected<std::vector<pack>, std::string> {
    fs::directory_iterator directory_it;
    try {
        directory_it = fs::directory_iterator(dir);
    } catch(const fs::filesystem_error& e) {
        return std::unexpected(e.what());
    }

    return directory_it //
         | std::views::filter([](const auto& entry) { return entry.is_directory(); })
         | std::views::transform([](const auto& entry) { return fs::directory_iterator(entry); })
         | std::views::join //
         | std::views::filter([](const fs::directory_entry& entry) {
               return entry.is_regular_file() && entry.path().extension() == ".toml";
           })
         | std::views::transform(pack_from_direntry)                           //
         | std::views::filter([](const auto& opt) { return opt.has_value(); }) //
         | std::views::transform([](const auto& opt) { return *opt; })
         | std::ranges::to<std::vector<pack>>();
}

} // namespace plg
