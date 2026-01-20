#include <catch2/catch_test_macros.hpp>

#include "module.hpp"
#include <sstream>

TEST_CASE("module_manifest deserialization") {
    const auto* toml = R"(
    name = "test"
    description = "A short description"
    )";

    auto stream = std::stringstream(toml);

    auto actual = plg::module_manifest::deserialize(stream);
    REQUIRE(actual.has_value());

    auto expected = plg::module_manifest{
        .name        = "test",
        .description = "A short description",
    };

    CHECK(*actual == expected);
}
