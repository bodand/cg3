/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-27.
 *
 * checks/fio/src/fio/file_routine_callback --
 */
#ifndef CG3_OPENER_CALLBACK_HXX
#define CG3_OPENER_CALLBACK_HXX

#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace cg3 {
    struct fio;

    struct opener_callback final : clang::ast_matchers::MatchFinder::MatchCallback {
        explicit opener_callback(fio* fio);

        void
        run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;

    private:
        fio* _fio;
    };
}

#endif
