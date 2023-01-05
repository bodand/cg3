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
    struct t final : check,
                     clang::ast_matchers::MatchFinder::MatchCallback {
        t();

        void
        run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

        void
        check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) override;

    private:
        unsigned _diag_id{};
        clang::ast_matchers::MatchFinder _finder{};
    };

    template<>
    struct loader<check_types::t> {
        using type = t;
    };
}

#endif
