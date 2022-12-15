/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-26.
 *
 * checks/globus/src/globus/global_var_callback --
 */
#ifndef CG3_GLOBAL_VAR_CALLBACK_HXX
#define CG3_GLOBAL_VAR_CALLBACK_HXX

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/Diagnostic.h>

namespace cg3 {
    struct globus;

    struct global_var_callback : clang::ast_matchers::MatchFinder::MatchCallback {
        explicit global_var_callback(globus* globus);

        void
        run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;

        void
        configure_engine(clang::DiagnosticsEngine& diag_engine);

    private:
        unsigned _diag_id;
        globus* _globus;
    };
}

#endif
