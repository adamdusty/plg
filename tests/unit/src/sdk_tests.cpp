#include <catch2/catch_test_macros.hpp>

#include <sdk/sdk.hpp>

TEST_CASE("Hello sdk") {
    REQUIRE(plg::version() == 0);
}
