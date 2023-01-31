/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2023. 01. 30.
 *
 * checks/fleak/test/check_spec --
 */

#include <concepts>

#include <catch2/catch_test_macros.hpp>
#include <chk3/check.hxx>
#include <fleak/fleak.hxx>

TEST_CASE("fleak conforms to check", "[fleak]") {
    STATIC_CHECK(std::convertible_to<cg3::fleak*, cg3::check*>);
}

TEST_CASE("fleak is constructible with diagnostics engine ptr", "[fleak]") {
    STATIC_CHECK(std::is_constructible_v<cg3::fleak, clang::DiagnosticsEngine*>);
}

TEST_CASE("loader<fleak>::type is fleak", "[fleak]") {
    STATIC_CHECK(std::same_as<cg3::loader<cg3::check_types::fleak>::type,
                              cg3::fleak>);
}
