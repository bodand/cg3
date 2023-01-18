/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 14.
 *
 * checks/bugmalloc/test/ast_tests --
 *   Tests for correctly bugmalloc reporting AST warnings.
 */

#include <filesystem>
#include <iostream>

#include <bugmalloc/bugmalloc.hxx>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <test-commons/capture_stream.hxx>
#include <test-commons/test_ast_loader.hxx>
#include <test-commons/test_consumer.hxx>

#include <clang/Tooling/Tooling.h>

namespace fs = std::filesystem;

TEST_CASE("files without calls to direct system malloc do not trigger warnings",
          "[bugmalloc]") {
    const std::string src = GENERATE("data/ok.c.ast",
                                     "data/ok.cxx.ast");
    REQUIRE(exists(fs::path(src)));
    cg3::test_ast_loader ldr(src);
    cg3::bugmalloc check;

    ldr.diag_sink->set_info_check([&]([[maybe_unused]] const clang::Diagnostic& x) {
        INFO(src);
        CHECK(false);
    });

    SECTION("doesn't cause any warnings") {
        check.check_ast(ldr.ast);
        CHECK(true);
    }

    SECTION("doesn't print output") {
        check.check_ast(ldr.ast);
        auto written = cg3::capture_stream<&std::cout>([&] {
            check.collected_report();
        });

        INFO(src);
        INFO(written);
        CHECK(written.empty());
    }
}

// NOLINTNEXTLINE test-macro
TEST_CASE("files without calls to direct any malloc do not trigger warnings but generate reports",
          "[bugmalloc]") {
    const std::string src = GENERATE("data/empty.c.ast",
                                     "data/empty.cxx.ast");
    REQUIRE(exists(fs::path(src)));
    cg3::test_ast_loader ldr(src);
    cg3::bugmalloc check;

    ldr.diag_sink->set_info_check([&]([[maybe_unused]] const clang::Diagnostic& x) {
        INFO(src);
        CHECK(false);
    });

    SECTION("doesn't cause any warnings") {
        check.check_ast(ldr.ast);
        CHECK(true);
    }

    SECTION("prints collected report about not having any memory mgmt") {
        check.check_ast(ldr.ast);
        auto written = cg3::capture_stream<&std::cout>([&] {
            check.collected_report();
        });

        INFO(src);
        INFO(written);
        CHECK_FALSE(written.empty());
        using Catch::Matchers::ContainsSubstring;
        CHECK_THAT(written, ContainsSubstring("does not"));
        CHECK_THAT(written, ContainsSubstring("allocate memory"));
    }
}

// NOLINTNEXTLINE test-macro
TEST_CASE("files with some calls to direct any malloc trigger bugmalloc",
          "[bugmalloc]") {
    const std::string src = GENERATE("data/partial.c.ast",
                                     "data/partial.cxx.ast");
    REQUIRE(exists(fs::path(src)));
    cg3::test_ast_loader ldr(src);
    cg3::bugmalloc check;

    SECTION("only causes one warning despite two calls to malloc") {
        int i = 0;
        ldr.diag_sink->set_info_check([&src, &i]([[maybe_unused]] const clang::Diagnostic& x) {
            INFO(src);
            CHECK(i == 0); // triggers only once (bad fn.)
            ++i;
        });

        check.check_ast(ldr.ast);
        CHECK(i == 1);
    }

    SECTION("prints collected report with bad not calling dbg_malloc") {
        check.check_ast(ldr.ast);
        auto written = cg3::capture_stream<&std::cout>([&] {
            check.collected_report();
        });

        INFO(src);
        INFO(written);
        CHECK_FALSE(written.empty());
        using Catch::Matchers::ContainsSubstring;
        CHECK_THAT(written, ContainsSubstring(ldr.get_source_filename()));
    }
}

// NOLINTNEXTLINE test-macro
TEST_CASE("files with all calls to direct any malloc trigger bugmalloc",
          "[bugmalloc]") {
    const std::string src = GENERATE("data/bad.c.ast",
                                     "data/bad.cxx.ast");
    REQUIRE(exists(fs::path(src)));
    cg3::test_ast_loader ldr(src);
    cg3::bugmalloc check;

    SECTION("two invalid calls generate two warnings") {
        int i = 0;
        ldr.diag_sink->set_info_check([&src, &i]([[maybe_unused]] const clang::Diagnostic& x) {
            INFO(src);
            ++i;
        });

        check.check_ast(ldr.ast);
        CHECK(i == 2);
    }

    SECTION("prints collected report with bad/bad2 not calling dbg_malloc") {
        check.check_ast(ldr.ast);
        auto written = cg3::capture_stream<&std::cout>([&] {
            check.collected_report();
        });

        INFO(src);
        INFO(written);
        CHECK_FALSE(written.empty());
        using Catch::Matchers::ContainsSubstring;
        CHECK_THAT(written, ContainsSubstring(ldr.get_source_filename()));
    }
}

// NOLINTNEXTLINE test-macro
TEST_CASE("files with tricky calls trigger bugmalloc",
          "[bugmalloc]") {
    const std::string src = GENERATE("data/tricky.c.ast",
                                     "data/tricky.cxx.ast");
    REQUIRE(exists(fs::path(src)));
    cg3::test_ast_loader ldr(src);
    cg3::bugmalloc check;

    SECTION("tricky call generates warnings") {
        int i = 0;
        ldr.diag_sink->set_info_check([&src, &i]([[maybe_unused]] const clang::Diagnostic& x) {
            INFO(src);
            ++i;
        });

        check.check_ast(ldr.ast);
        CHECK(i == 1);
    }

    SECTION("prints collected report tricky functions") {
        check.check_ast(ldr.ast);
        auto written = cg3::capture_stream<&std::cout>([&] {
            check.collected_report();
        });

        INFO(src);
        INFO(written);
        CHECK_FALSE(written.empty());
        using Catch::Matchers::ContainsSubstring;
        CHECK_THAT(written, ContainsSubstring(ldr.get_source_filename()));
        CHECK_THAT(written, ContainsSubstring("tricky"));
    }
}
