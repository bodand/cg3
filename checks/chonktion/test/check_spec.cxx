/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2023. 01. 14.
 *
 * checks/bugmalloc/test/check_spec --
 */

#include <concepts>

#include <catch2/catch_test_macros.hpp>
#include <chk3/check.hxx>
#include <chonktion/chonktion.hxx>

TEST_CASE("chonktion conforms to check", "[chonktion]") {
    STATIC_CHECK(std::convertible_to<cg3::chonktion*, cg3::check*>);
}

TEST_CASE("chonktion is constructible with diagnostics engine ptr", "[chonktion]") {
    STATIC_CHECK(std::is_constructible_v<cg3::chonktion, clang::DiagnosticsEngine*>);
}

TEST_CASE("loader<chonktion>::type is chonktion", "[chonktion]") {
    STATIC_CHECK(std::same_as<cg3::loader<cg3::check_types::chonktion>::type,
                              cg3::chonktion>);
}
