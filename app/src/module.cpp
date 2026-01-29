#include "module.hpp"

#include "constants.hpp"
#include "parsing.hpp"
#include <filesystem>
#include <fstream>
#include <ranges>
#include <toml++/toml.hpp>

namespace fs = std::filesystem;

namespace {
auto module_from_direntry(const fs::directory_entry& entry) -> std::optional<plg::module> {
    auto manifest_file_path = entry.path() / plg::MODULE_MANIFEST_FILE_NAME;
    auto file               = std::ifstream(manifest_file_path);
    if(!file) {
        return std::nullopt;
    }

    toml::table table;

    try {
        table = toml::parse(file);
    } catch(const toml::parse_error& e) {
        return std::nullopt;
    }

    auto man = plg::parsing::from_toml<plg::module_manifest>(table);
    if(!man.has_value()) {
        return std::nullopt;
    }

    return plg::module{
        .manifest  = std::move(*man),
        .directory = entry.path(),
    };
}
} // namespace

namespace plg {

auto find_modules(const std::string& dir) -> std::expected<std::vector<module>, std::string> {
    fs::directory_iterator directory_it;
    try {
        directory_it = fs::directory_iterator(dir);
    } catch(const fs::filesystem_error& e) {
        return std::unexpected(e.what());
    }

    return directory_it
         /* Get namespace subdirectories*/
         | std::views::filter([](const auto& entry) { return entry.is_directory(); })
         /* Directory iterator for each namespace dir*/
         | std::views::transform([](const auto& entry) { return fs::directory_iterator(entry); })
         /* Flatten directories */
         | std::views::join //
         /* In each namespace subdirectory, look for nspace/subdir/manifest.toml*/
         | std::views::filter([](const fs::directory_entry& entry) {
               return fs::exists(entry.path() / MODULE_MANIFEST_FILE_NAME);
           })
         /* Get manifest from nspace/subdir/manifest.toml*/
         | std::views::transform(module_from_direntry)
         /* Filter out manifests that couldn't be parsed */
         | std::views::filter([](const auto& opt) { return opt.has_value(); }) //
         /* Dereference optionals */
         | std::views::transform([](const auto& opt) { return *opt; })
         | std::ranges::to<std::vector<module>>();

    // return directory_it //
    //      | std::views::filter([](const auto& entry) {
    //            return fs::exists(entry.path() / MODULE_MANIFEST_FILE_NAME);
    //        })
    //      | std::views::transform(module_from_direntry)
    //      | std::views::filter([](const auto& mod) { return mod.has_value(); })
    //      | std::views::transform([](const auto& mod) { return mod.value(); })
    //      | std::ranges::to<std::vector<module>>();
}

} // namespace plg
