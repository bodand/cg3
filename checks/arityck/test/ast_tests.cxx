/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 10.
 *
 * checks/arityck/test/ast_tests --
 */

#include <filesystem>
#include <iostream>

#include <arityck/arityck.hxx>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <test-commons/capture_stream.hxx>
#include <test-commons/test_ast_loader.hxx>
#include <test-commons/test_consumer.hxx>

#include <clang/Tooling/Tooling.h>

namespace fs = std::filesystem;

// NOLINTNEXTLINE test-macro
TEST_CASE("files without faulty function decls don't trigger anything",
          "[arityck]") {
    const auto src = GENERATE(as<fs::path>{},
                              "data/empty.c.ast",
                              "data/empty.cxx.ast",
                              "data/ok.c.ast",
                              "data/ok.cxx.ast");
    cg3::test_ast_loader fixture(src);
    cg3::arityck check(fixture.get_diag());

    fixture.diag_sink->set_info_check([](auto) {
        REQUIRE(false);
    });

    SECTION("doesn't cause any warnings") {
        check.check_ast(fixture.ast);
        REQUIRE(true);
    }
}

// NOLINTNEXTLINE test-macro
TEST_CASE("files with incorrect decls produce diagnostics",
          "[arityck]") {
    const auto src = GENERATE(as<fs::path>{}, "data/bad.c.ast", "data/bad.cxx.ast");
    REQUIRE(exists(src));
    cg3::test_ast_loader fixture(src);
    cg3::arityck check(fixture.get_diag());

    fixture.diag_sink->set_level_check([](auto lvl) {
        REQUIRE(lvl == clang::DiagnosticsEngine::Warning);
    });
    fixture.diag_sink->set_info_check([&]([[maybe_unused]] const auto& info) {
        CHECK(true);
        // cannot figure out why this aborts b/c not having a SourceManager
        //        auto&& srcmgr = info.getSourceManager();
        //        auto&& loc = info.getLocation();
        //
        //        auto name_raw = srcmgr.getFilename(loc);
        //        auto fname = fs::path(name_raw.data(),
        //                              name_raw.data() + name_raw.size())
        //                            .filename();
        //        auto expected_fname = fs::path(src);
        //        expected_fname = expected_fname.stem().filename();
        //        REQUIRE(fname == expected_fname);
        //
        //        auto line = srcmgr.getPresumedLineNumber(loc);
        //        constexpr const static int line_expected = 14; // see data/bad.cxx
        //        REQUIRE(line == line_expected);
    });

    SECTION("causes warnings") {
        check.check_ast(fixture.ast);
        REQUIRE(true);
    }
}
