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
#include <utility>

#include <arityck/arityck.hxx>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>

namespace fs = std::filesystem;

namespace {
    struct test_consumer : clang::DiagnosticConsumer {
        void
        HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                         const clang::Diagnostic& Info) override {
            _level_check(DiagLevel);
            _info_check(Info);
        }

        template<class Fn>
        void
        set_level_check(Fn&& fn) {
            _level_check = std::forward<Fn>(fn);
        }

        template<class Fn>
        void
        set_info_check(Fn&& fn) {
            _info_check = std::forward<Fn>(fn);
        }

    private:
        std::function<void(clang::DiagnosticsEngine::Level)> _level_check{[](const auto&) {
        }};
        std::function<void(const clang::Diagnostic&)> _info_check{[](const auto&) {
        }};
    };

    struct reset_stream {
        reset_stream(const reset_stream&) = delete;
        reset_stream(reset_stream&&) noexcept = delete;
        reset_stream&
        operator=(const reset_stream&) = delete;
        reset_stream&
        operator=(reset_stream&&) noexcept = delete;

        explicit reset_stream(std::ostream* ostrm, std::streambuf* buf)
             : _ostrm(ostrm),
               _buf(buf) { }

        ~reset_stream() noexcept {
            _ostrm->rdbuf(_buf);
        }

    private:
        std::ostream* _ostrm;
        std::streambuf* _buf;
    };

    template<std::ostream* OStrm, class Fn, class... Args>
    std::string
    capture_stream(Fn&& fn, Args&&... args) {
        const std::ostringstream ss;
        { // REDIRECTION SCOPE
            auto buf_buf [[maybe_unused]] = reset_stream(OStrm, OStrm->rdbuf(ss.rdbuf()));
            std::forward<Fn>(fn)(std::forward<Args>(args)...);
        }
        return ss.str();
    }

    struct ast_loader_fixture {
        clang::CompilerInstance ci{};
        std::unique_ptr<test_consumer> diag_sink = std::make_unique<test_consumer>();
        std::vector<std::unique_ptr<clang::ASTUnit>> ast{};

        explicit ast_loader_fixture(std::string fname)
             : ast_filename(std::move(fname)) {
            ci.createDiagnostics(diag_sink.get(), false);
            ast.emplace_back(load_ast());
        }

    private:
        llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diag_opts =
               new clang::DiagnosticOptions();
        llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diag_id =
               new clang::DiagnosticIDs();
        std::string ast_filename;
        llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> diag =
               new clang::DiagnosticsEngine(diag_id, diag_opts.get(), diag_sink.get(), false);

        std::unique_ptr<clang::ASTUnit>
        load_ast() {
            return clang::ASTUnit::LoadFromASTFile(ast_filename,
                                                   ci.getPCHContainerReader(),
                                                   clang::ASTUnit::LoadEverything,
                                                   diag,
                                                   ci.getFileSystemOpts());
        };
    };
}

// NOLINTNEXTLINE test-macro
TEST_CASE("files without faulty function decls don't trigger anything",
          "[arityck]") {
    const std::string src = GENERATE("data/empty.c.ast",
                                     "data/empty.cxx.ast",
                                     "data/ok.c.ast",
                                     "data/ok.cxx.ast");
    ast_loader_fixture fixture(src);
    cg3::arityck check;

    fixture.diag_sink->set_info_check([](auto) {
        REQUIRE(false);
    });

    SECTION("doesn't cause any warnings") {
        check.check_ast(fixture.ast);
        REQUIRE(true);
    }

    SECTION("doesn't print output") {
        auto written = capture_stream<&std::cout>([&] {
            check.collected_report();
        });

        REQUIRE(written.empty());
    }
}

// NOLINTNEXTLINE test-macro
TEST_CASE("files with incorrect decls produce diagnostics",
          "[arityck]") {
    const fs::path src = GENERATE("data/bad.c.ast", "data/bad.cxx.ast");
    ast_loader_fixture fixture(src.string());
    cg3::arityck check;

    fixture.diag_sink->set_level_check([](auto lvl) {
        REQUIRE(lvl == clang::DiagnosticsEngine::Warning);
    });
    fixture.diag_sink->set_info_check([&](const auto& info) {
        auto&& srcmgr = info.getSourceManager();
        auto&& loc = info.getLocation();

        auto name_raw = srcmgr.getFilename(loc);
        auto fname = fs::path(name_raw.data(),
                              name_raw.data() + name_raw.size())
                            .filename();
        auto expected_fname = fs::path(src);
        expected_fname = expected_fname.stem().filename();
        REQUIRE(fname == expected_fname);

        auto line = srcmgr.getPresumedLineNumber(loc);
        constexpr const static int line_expected = 14; // see data/bad.cxx
        REQUIRE(line == line_expected);
    });

    SECTION("causes warnings") {
        check.check_ast(fixture.ast);
        REQUIRE(true);
    }

    SECTION("produces correctly formatted output") {
        check.check_ast(fixture.ast);
        auto written = capture_stream<&std::cout>([&] {
            check.collected_report();
        });

        using Catch::Matchers::ContainsSubstring;
        REQUIRE_THAT(written, ContainsSubstring(src.filename().stem().string()));
        REQUIRE_THAT(written, ContainsSubstring(":14")); // see data/bad.cxx
        REQUIRE_THAT(written, ContainsSubstring("quinary"));
    }
}
