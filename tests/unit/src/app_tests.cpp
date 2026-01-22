#include <catch2/catch_test_macros.hpp>

#include "dependency.hpp"
#include "module.hpp"
#include "parsing.hpp"
#include "version.hpp"
#include <toml++/toml.hpp>

TEST_CASE("version parsing") {
    SECTION("With annotation") {
        const auto* ver = "1.0.0-annotation";
        auto res        = plg::version::parse(ver);
        REQUIRE(res.has_value());
        CHECK(res->major == 1);
        CHECK(res->minor == 0);
        CHECK(res->patch == 0);
        CHECK(res->annotation.has_value());
        CHECK(res->annotation.value() == "annotation");
    }

    SECTION("Without annotation") {
        const auto* ver = "3.1.4";
        auto res        = plg::version::parse(ver);
        REQUIRE(res.has_value());
        CHECK(res->major == 3);
        CHECK(res->minor == 1);
        CHECK(res->patch == 4);
        CHECK(!res->annotation.has_value());
    }

    SECTION("Invalid major") {
        const auto* ver = "hello.1.4";
        auto res        = plg::version::parse(ver);
        REQUIRE(!res.has_value());
        CHECK(res.error() == "Failed to parse version");
    }

    SECTION("Invalid minor") {
        const auto* ver = "3.a.4";
        auto res        = plg::version::parse(ver);
        REQUIRE(!res.has_value());
        CHECK(res.error() == "Failed to parse version");
    }

    SECTION("Invalid patch") {
        const auto* ver = "3.5.a";
        auto res        = plg::version::parse(ver);
        REQUIRE(!res.has_value());
        CHECK(res.error() == "Failed to parse version");
    }

    SECTION("Annotation has invalid char") {
        const auto* ver = "3.5.6-$";
        auto res        = plg::version::parse(ver);
        REQUIRE(!res.has_value());
        CHECK(res.error() == "Failed to parse version");
    }
}

TEST_CASE("version equality") {
    SECTION("major") {
        auto ver1 = plg::version(1, 1, 1, "pre");
        auto ver2 = plg::version(2, 1, 1, "pre");

        CHECK(ver2 > ver1);
        CHECK(ver1 < ver2);
    }

    SECTION("minor") {
        auto ver1 = plg::version(1, 1, 2, "pre");
        auto ver2 = plg::version(1, 2, 1, "pre");

        CHECK(ver2 > ver1);
        CHECK(ver1 < ver2);
    }

    SECTION("patch") {
        auto ver1 = plg::version(1, 1, 1, "pre");
        auto ver2 = plg::version(1, 1, 2, "pre");

        CHECK(ver2 > ver1);
        CHECK(ver1 < ver2);
    }

    SECTION("equal") {
        auto ver1 = plg::version(1, 1, 1, "pre");
        auto ver2 = plg::version(1, 1, 1, "post");

        CHECK(ver1 == ver2);
    }
}

TEST_CASE("dependency from toml", "[app]") {
    SECTION("dependency") {
        auto node = toml::table{
            {"namespace", "test"},
            {"name", "module"},
            {"version", "1.2.3-pre-release"},
        };

        auto res = plg::parsing::from_toml<plg::dependency>(node);
        REQUIRE(res.has_value());

        auto expected = plg::dependency{
            .nspace  = "test",
            .name    = "module",
            .version = {1, 2, 3, "pre-release"},
        };

        CHECK(*res == expected);
    }

    SECTION("toml node is not a table") {
        auto node = toml::array{};

        auto res = plg::parsing::from_toml<plg::dependency>(node);
        CHECK(not res.has_value());
        CHECK(res.error() == "Expected toml node to be table");
    }

    SECTION("node is missing fields") {
        auto node = toml::table{};

        auto res = plg::parsing::from_toml<plg::dependency>(node);
        CHECK(not res.has_value());
        CHECK(res.error() == "Missing required field: `namespace`");

        node.emplace("namespace", std::string("test"));
        res = plg::parsing::from_toml<plg::dependency>(node);
        CHECK(not res.has_value());
        CHECK(res.error() == "Missing required field: `name`");

        node.emplace("name", std::string("module"));
        res = plg::parsing::from_toml<plg::dependency>(node);
        CHECK(not res.has_value());
        CHECK(res.error() == "Missing required field: `version`");

        node.emplace("version", std::string("1.2.3"));
        res = plg::parsing::from_toml<plg::dependency>(node);
        CHECK(res.has_value());
    }
}

TEST_CASE("module manifest from toml", "[app]") {
    SECTION("module_manifest") {
        auto node = toml::table{
            {"namespace", "test"},
            {"name", "test-module"},
            {"version", "3.1.2-pre-release"},
            {"short_description", "A test module"},
            {"long_description", "A longer description of this test module"},
            {
                "dependencies",
                toml::array{
                    toml::table{
                        {"namespace", "test"},
                        {"name", "test-mod-dep1"},
                        {"version", "1.0.0"},
                    },
                    toml::table{
                        {"namespace", "test"},
                        {"name", "test-mod-dep2"},
                        {"version", "0.1.0"},
                    },
                },
            },
        };

        auto expected = plg::module_manifest{
            .nspace            = "test",
            .name              = "test-module",
            .version           = {3, 1, 2, "pre-release"},
            .short_description = "A test module",
            .long_description  = "A longer description of this test module",
            .dependencies      = std::vector<plg::dependency>{
                {"test", "test-mod-dep1", {1, 0, 0}},
                {"test", "test-mod-dep2", {0, 1, 0}},
            },
        };

        auto result = plg::parsing::from_toml<plg::module_manifest>(node);
        REQUIRE(result.has_value());
        CHECK(*result == expected);
    }

    SECTION("toml node is not a table") {
        auto node = toml::array{};

        auto res = plg::parsing::from_toml<plg::module_manifest>(node);
        CHECK(not res.has_value());
        CHECK(res.error() == "Expected toml node to be table");
    }

    SECTION("node is missing fields") {
        auto node = toml::table{};

        auto res = plg::parsing::from_toml<plg::module_manifest>(node);
        CHECK(not res.has_value());
        CHECK(res.error() == "Missing or invalid field: `namespace`");

        node.emplace("namespace", std::string("test"));
        res = plg::parsing::from_toml<plg::module_manifest>(node);
        CHECK(not res.has_value());
        CHECK(res.error() == "Missing or invalid field: `name`");

        node.emplace("name", std::string("module"));
        res = plg::parsing::from_toml<plg::module_manifest>(node);
        CHECK(not res.has_value());
        CHECK(res.error() == "Missing or invalid field: `version`");

        node.emplace("version", std::string("1.2.3"));
        res = plg::parsing::from_toml<plg::module_manifest>(node);
        CHECK(res.has_value());
    }
}
