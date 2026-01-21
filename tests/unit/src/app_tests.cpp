#include <catch2/catch_test_macros.hpp>

#include "module.hpp"
#include <sstream>

TEST_CASE("module_manifest deserialization") {
    SECTION("Valid manifest") {
        const auto* toml = R"(
        namespace = "test"
        name = "test"
        description = "A short description"
        )";

        auto stream = std::stringstream(toml);

        auto actual = plg::module_manifest::deserialize(stream);
        REQUIRE(actual.has_value());

        auto expected = plg::module_manifest{
            .nspace      = "test",
            .name        = "test",
            .description = "A short description",
        };

        CHECK(*actual == expected);
    }

    SECTION("Manifest missing namespace") {
        const auto* toml = R"(
        name = "test"
        description = "A short description"
        )";

        auto stream = std::stringstream(toml);

        auto actual = plg::module_manifest::deserialize(stream);
        CHECK(!actual.has_value());
    }

    SECTION("Manifest missing name") {
        const auto* toml = R"(
        namespace = "test"
        description = "A short description"
        )";

        auto stream = std::stringstream(toml);

        auto actual = plg::module_manifest::deserialize(stream);
        CHECK(!actual.has_value());
    }

    SECTION("Manifest missing description") {
        const auto* toml = R"(
        namespace = "test"
        name = "test"
        )";

        auto stream = std::stringstream(toml);

        auto actual = plg::module_manifest::deserialize(stream);
        CHECK(!actual.has_value());
    }
}
