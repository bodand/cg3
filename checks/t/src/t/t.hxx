/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 04.
 *
 * checks/t/src/t/t --
 */

#ifndef CG3_T_HXX
#define CG3_T_HXX

#include <chk3/check.hxx>
#include <chk3/loader.hxx>

#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace cg3 {
    struct t final : typed_check<check_types::t>,
                     clang::ast_matchers::MatchFinder::MatchCallback {
        explicit t(clang::DiagnosticsEngine* diag);

        void
        run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

    protected:
        void
        match_ast(clang::ASTContext& context) override;

    private:
        check_diagnostic _invalid_fopen_diag;
        clang::ast_matchers::MatchFinder _finder{};
    };

    template<>
    struct loader<check_types::t> {
        using type = t;
    };
}

#endif
