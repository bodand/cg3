/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 11.
 *
 * src/util/test-commons/test_ast_loader --
 */

#ifndef CG3_TEST_AST_LOADER_HXX
#define CG3_TEST_AST_LOADER_HXX

#include <cassert>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <test-commons/test_consumer.hxx>

#include <clang/AST/AST.h>
#include <clang/Frontend/CompilerInstance.h>

namespace cg3 {
    struct test_ast_loader {
        clang::CompilerInstance ci{};
        std::unique_ptr<cg3::test_consumer> diag_sink = std::make_unique<cg3::test_consumer>();
        std::vector<std::unique_ptr<clang::ASTUnit>> ast{};

        std::string
        get_source_filename() {
            return ast_filename.filename().stem().string();
        }

        explicit test_ast_loader(const std::string& fname)
             : ast_filename(fname) {
            assert(ast_filename.extension() == ".ast");

            auto fpath = std::filesystem::path(ast_filename).parent_path();
            ci.getHeaderSearchOpts().AddPath(fpath.string(),
                                             clang::frontend::IncludeDirGroup::Quoted,
                                             false,
                                             false);
            ci.createDiagnostics(diag_sink.get(), false);
            ast.emplace_back(load_ast());
        }

    private:
        llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diag_opts =
               new clang::DiagnosticOptions();
        llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diag_id =
               new clang::DiagnosticIDs();
        std::filesystem::path ast_filename;
        llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> diag =
               new clang::DiagnosticsEngine(diag_id, diag_opts.get(), diag_sink.get(), false);

        std::unique_ptr<clang::ASTUnit>
        load_ast() {
            return clang::ASTUnit::LoadFromASTFile(ast_filename.string(),
                                                   ci.getPCHContainerReader(),
                                                   clang::ASTUnit::LoadEverything,
                                                   diag,
                                                   ci.getFileSystemOpts());
        };
    };
}

#endif
