#include "module.hpp"

#include <algorithm>
#include <filesystem>
#include <format>
#include <istream>
#include <toml++/toml.hpp>

namespace fs = std::filesystem;

namespace {

constexpr auto dependency_from_toml(const toml::node& node)
    -> std::expected<plg::dependency, std::string> {
    if(not node.is_table()) {
        return std::unexpected("Node not a table");
    }

    const auto* tbl = node.as_table();
    auto nspace     = (*tbl)["namespace"].value<std::string>();
    auto name       = (*tbl)["name"].value<std::string>();
    auto version    = (*tbl)["string"].value<std::string>();

    if(not nspace or not name or not version) {
        return std::unexpected("Missing required field");
    }

    auto ver = plg::version::parse(*version);
    if(not ver) {
        std::unexpected("Failed to parse version");
    }

    return plg::dependency{.nspace = *nspace, .name = *name, .version = *ver};
}

} // namespace

namespace plg {

constexpr auto module_manifest::parse(std::istream& toml)
    -> std::expected<module_manifest, std::string> {
    auto table = toml::table{};
    try {
        table = toml::parse(toml);
    } catch(const toml::parse_error& e) {
        return std::unexpected(e.what());
    }

    auto nspace = table["namespace"].value<std::string>();
    if(!nspace) {
        return std::unexpected("Missing required field: `namespace`");
    }

    auto name = table["name"].value<std::string>();
    if(!name) {
        return std::unexpected("Missing required field: `name`");
    }

    auto sdesc = table["short_description"].value<std::string>();
    auto ldesc = table["long_description"].value<std::string>();

    auto dependencies = std::vector<dependency>();
    if(auto* deps = table["dependencies"].as_array()) {
        deps->for_each([&](auto&& node) {
            if(auto dep = dependency_from_toml(node)) {
                dependencies.emplace_back(*dep);
            }
        });
    }

    return module_manifest{
        .nspace            = *nspace,
        .name              = *name,
        .short_description = sdesc,
        .long_description  = ldesc,
        .dependencies =
    };
}

auto find_modules(const std::string& dir) -> std::expected<std::vector<module>, std::string> {
    return std::unexpected("unimpl");
    // fs::directory_iterator directory_it;
    // try {
    //     directory_it = fs::directory_iterator(dir);
    // } catch(const fs::filesystem_error& e) {
    //     return std::unexpected(e.what());
    // }

    // // Get sub-directories of 'dir' that contain a manifest.json
    // auto module_directories = std::vector<fs::directory_entry>();
    // std::copy_if(
    //     begin(directory_it),
    //     end(directory_it),
    //     std::back_inserter(module_directories),
    //     [](const fs::directory_entry& entry) {
    //         return entry.is_directory() && fs::exists(entry.path() / "manifest.toml");
    //     }
    // );

    // auto modules = std::vector<module_find_result>();
    // std::transform(
    //     module_directories.begin(),
    //     module_directories.end(),
    //     std::back_inserter(modules),
    //     [](const fs::directory_entry& entry) -> module_find_result {
    //         auto manifest_file_path = entry.path() / "manifest.toml";
    //         auto file               = std::ifstream(manifest_file_path);
    //         if(!file) {
    //             auto msg =
    //                 std::format("Unable to open manifest.toml at: {}", entry.path().string());
    //             return std::unexpected(module_find_error{msg});
    //         }

    //         auto man = module_manifest::deserialize(file);
    //         if(!man.has_value()) {
    //             return std::unexpected(module_find_error{man.error()});
    //         }

    //         return module{
    //             .nspace      = man->nspace,
    //             .name        = man->name,
    //             .description = man->description,
    //             .directory   = entry.path().generic_string(),
    //         };
    //     }
    // );

    // return modules;
}

} // namespace plg
