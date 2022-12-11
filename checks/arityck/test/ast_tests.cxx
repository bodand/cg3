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
#include <boost/ut.hpp>

#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
using namespace boost::ut;
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
        std::ostringstream ss;
        { // REDIRECTION SCOPE
            auto buf_buf [[maybe_unused]] = reset_stream(OStrm, OStrm->rdbuf(ss.rdbuf()));
            std::forward<Fn>(fn)(std::forward<Args>(args)...);
        }
        return ss.str();
    }
}

[[maybe_unused]] const suite ast_tests_suite = [] {
    clang::CompilerInstance ci;
    llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diag_opts =
           new clang::DiagnosticOptions();
    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diag_id =
           new clang::DiagnosticIDs();

    auto load_ast = [&](const std::string& file_path, const auto& diag) {
        return clang::ASTUnit::LoadFromASTFile(file_path,
                                               ci.getPCHContainerReader(),
                                               clang::ASTUnit::LoadEverything,
                                               diag,
                                               ci.getFileSystemOpts());
    };

    "file without function decls doesn't cause warnings [empty.cxx]"_test = [&](auto ast_file) {
        auto diag_sink = new test_consumer;
        ci.createDiagnostics(diag_sink, false);
        llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> diag =
               new clang::DiagnosticsEngine(diag_id, diag_opts.get(), diag_sink);
        cg3::arityck check;

        std::vector<std::unique_ptr<clang::ASTUnit>> ast;
        ast.emplace_back(load_ast(ast_file, diag));

        "check doesn't raise warnings during processing"_test = [&] {
            diag_sink->set_info_check([](auto lvl) {
                expect(false) << "callback should not have been called";
            });

            check.check_ast(ast);
            expect(that % true);
        };

        "check does not produce output in summary"_test = [&] {
            auto written = capture_stream<&std::cout>([&] {
                check.collected_report();
            });

            expect(written.empty());
        };
    } | std::vector{"data/empty.c.ast", "data/empty.cxx.ast"};

    "file with correct function decls doesn't cause warnings [ok]"_test = [&](auto ast_file) {
        auto diag_sink = new test_consumer;
        ci.createDiagnostics(diag_sink, false);
        llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> diag =
               new clang::DiagnosticsEngine(diag_id, diag_opts.get(), diag_sink);
        cg3::arityck check;

        std::vector<std::unique_ptr<clang::ASTUnit>> ast;
        ast.emplace_back(load_ast(ast_file, diag));

        "check doesn't raise warnings during processing"_test = [&] {
            diag_sink->set_level_check([](auto lvl) {
                expect(false) << "callback should not have been called";
            });

            check.check_ast(ast);
            expect(that % true);
        };

        "check does not produce output in summary"_test = [&] {
            auto written = capture_stream<&std::cout>([&] {
                check.collected_report();
            });

            expect(written.empty());
        };
    } | std::vector{"data/ok.c.ast", "data/ok.cxx.ast"};

    "file with failed function decls causes warnings [bad]"_test = [&](auto ast_file) {
        auto diag_sink = new test_consumer;
        ci.createDiagnostics(diag_sink, false);
        llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> diag =
               new clang::DiagnosticsEngine(diag_id, diag_opts.get(), diag_sink);
        cg3::arityck check;

        std::vector<std::unique_ptr<clang::ASTUnit>> ast;
        ast.emplace_back(load_ast(ast_file, diag));

        "check raises warnings during processing"_test = [&] {
            diag_sink->set_level_check([](auto lvl) {
                expect(that % lvl == clang::DiagnosticsEngine::Warning);
            });
            diag_sink->set_info_check([&](const auto& info) {
                auto&& srcmgr = info.getSourceManager();
                auto&& loc = info.getLocation();

                auto name_raw = srcmgr.getFilename(loc);
                auto fname = fs::path(name_raw.data(),
                                      name_raw.data() + name_raw.size())
                                    .filename();
                auto expected_fname = fs::path(ast_file);
                expected_fname = expected_fname.stem().filename();
                expect(that % fname == expected_fname);

                auto line = srcmgr.getPresumedLineNumber(loc);
                expect(that % line == 14); // see data/bad.cxx
            });

            check.check_ast(ast);
            expect(that % true);
        };

        "check produces summary"_test = [&] {
            auto written = capture_stream<&std::cout>([&] {
                check.collected_report();
            });

            expect(that % written.find("bad.c") != written.npos);
            expect(that % written.find(":14") != written.npos);
            expect(that % written.find("quinary") != written.npos);
        };
    } | std::vector{"data/bad.c.ast", "data/bad.cxx.ast"};
};
