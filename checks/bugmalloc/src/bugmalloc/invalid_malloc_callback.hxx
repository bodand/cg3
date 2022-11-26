/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-26.
 *
 * checks/bugmalloc/src/bugmalloc/invalid_malloc_callback --
 */
#ifndef CG3_INVALID_MALLOC_CALLBACK_HXX
#define CG3_INVALID_MALLOC_CALLBACK_HXX

#include <unordered_set>

#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace cg3 {
    struct bugmalloc;

    struct invalid_malloc_callback final : clang::ast_matchers::MatchFinder::MatchCallback {
        explicit invalid_malloc_callback(bugmalloc* check);

        void
        run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

    private:
        bugmalloc* _check;
        std::unordered_set<std::string> _bad_files;
    };
}

#endif
