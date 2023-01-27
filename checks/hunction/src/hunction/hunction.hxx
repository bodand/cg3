/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 04.
 *
 * checks/hunction/src/hunction/hunction --
 */

#ifndef CG3_HUNCTION_HXX
#define CG3_HUNCTION_HXX

#include <filesystem>

#include <cg3-common/hash_storages.hxx>
#include <chk3/check.hxx>
#include <chk3/loader.hxx>

#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace cg3 {
    struct hunction final : check,
                            clang::ast_matchers::MatchFinder::MatchCallback {
        explicit hunction(clang::DiagnosticsEngine* diag);

        void
        run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

        void
        collected_report() override;

    protected:
        void
        match_ast(clang::ASTContext& context) override;

    private:
        check_diagnostic _header_def_fun_diag;
        std::unordered_multimap<std::filesystem::path, std::string> _header_functions{};
        clang::ast_matchers::MatchFinder _finder{};
    };

    template<>
    struct loader<check_types::hunction> {
        using type = hunction;
    };
}

#endif
