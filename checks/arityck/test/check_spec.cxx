/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 08.
 *
 * checks/arityck/test/check_spec --
 */

#include <concepts>

#include <arityck/arityck.hxx>
#include <catch2/catch_test_macros.hpp>
#include <chk3/check.hxx>

TEST_CASE("arityck conforms to check", "[arityck]") {
    REQUIRE(std::convertible_to<cg3::arityck*, cg3::check*>);
}

TEST_CASE("arityck is default constructible", "[arityck]") {
    REQUIRE(std::is_default_constructible_v<cg3::arityck>);
}

TEST_CASE("loader<arityck>::type is arityck", "[arityck]") {
    REQUIRE(std::same_as<cg3::loader<cg3::check_types::arityck>::type,
                         cg3::arityck>);
}
