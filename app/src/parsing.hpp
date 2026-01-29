#pragma once

#include "dependency.hpp"
#include "module.hpp"
#include "pack.hpp"
#include <expected>
#include <string>
#include <toml++/toml.hpp>

namespace plg::parsing {

template<typename T>
auto from_toml(const toml::node&) -> std::expected<T, std::string>;

template<>
auto from_toml<dependency>(const toml::node& node) -> std::expected<dependency, std::string>;

template<>
auto from_toml<module_manifest>(const toml::node& node)
    -> std::expected<module_manifest, std::string>;

template<>
auto from_toml<pack>(const toml::node& node) -> std::expected<pack, std::string>;

} // namespace plg::parsing
