#include <catch2/catch_test_macros.hpp>

#include "dependency.hpp"
#include "module.hpp"
#include "version.hpp"
#include <sstream>

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

TEST_CASE("module_manifest deserialization") {
    constexpr auto toml = R"(
        namespace = "test"
        name = "test-module"
        version = "3.1.2-pre-release"
        short_description = "A test module"
        long_description = "A longer description of this test module"
        
        dependencies = [
            {
                namespace = "test",
                name = "test-mod-dep1",
                version = "1.0.0",
            },
            {
                namespace = "test",
                name = "test-mod-dep2",
                version = "0.1.0",
            }
        ]
    )";

    auto stream = std::stringstream(toml);
    auto actual = plg::module_manifest::parse(stream);
    REQUIRE(actual.has_value());

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

    CHECK(*actual == expected);
}
