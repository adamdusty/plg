#include "parsing.hpp"

namespace plg::parsing {

template<>
auto from_toml<dependency>(const toml::node& node) -> std::expected<dependency, std::string> {

    const auto* tbl = node.as_table();
    if(tbl == nullptr) {
        return std::unexpected("Expected toml node to be table");
    }

    auto get_string = [&](std::string_view key) -> std::expected<std::string, std::string> {
        if(const auto* val = (*tbl)[key].as_string()) {
            return std::string{val->get()};
        }
        return std::unexpected("Missing required field: `" + std::string(key) + "`");
    };

    auto nspace = get_string("namespace");
    if(!nspace) {
        return std::unexpected(nspace.error());
    }

    auto name = get_string("name");
    if(!name) {
        return std::unexpected(name.error());
    }

    auto version_str = get_string("version");
    if(!version_str) {
        return std::unexpected(version_str.error());
    }

    auto version = plg::version::parse(*version_str);
    if(!version) {
        return std::unexpected("Failed to parse version");
    }

    return dependency{
        .nspace  = std::move(*nspace),
        .name    = std::move(*name),
        .version = std::move(*version),
    };
}

template<>
auto from_toml<module_manifest>(const toml::node& node)
    -> std::expected<module_manifest, std::string> {

    const auto* tbl = node.as_table();
    if(tbl == nullptr) {
        return std::unexpected("Expected toml node to be table");
    }

    auto get_string = [&](std::string_view key) -> std::expected<std::string, std::string> {
        if(const auto* val = (*tbl)[key].as_string()) {
            return std::string{val->get()};
        }
        return std::unexpected("Missing or invalid field: `" + std::string(key) + "`");
    };

    auto nspace = get_string("namespace");
    if(!nspace) {
        return std::unexpected(nspace.error());
    }

    auto name = get_string("name");
    if(!name) {
        return std::unexpected(name.error());
    }

    auto version = get_string("version").and_then(
        [](const auto& str) -> std::expected<plg::version, std::string> {
            return version::parse(str);
        }
    );
    if(!version) {
        return std::unexpected(version.error());
    }

    auto sdesc = tbl->at_path("short_description").value<std::string>();
    auto ldesc = tbl->at_path("long_description").value<std::string>();

    auto dependencies            = std::vector<dependency>();
    const auto* dependency_array = tbl->at_path("dependencies").as_array();
    if(dependency_array != nullptr) {
        for(const auto& dep_node: *dependency_array) {
            if(auto dep = from_toml<dependency>(dep_node)) {
                dependencies.emplace_back(std::move(*dep));
            }
        }
    }

    return module_manifest{
        .nspace            = std::move(*nspace),
        .name              = std::move(*name),
        .version           = std::move(*version),
        .short_description = std::move(sdesc),
        .long_description  = std::move(ldesc),
        .dependencies      = std::move(dependencies),
    };
}

} // namespace plg::parsing
