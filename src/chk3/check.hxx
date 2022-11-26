/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-25.
 *
 * src/cg3/check --
 */
#ifndef CG3_CHECK_HXX
#define CG3_CHECK_HXX

#include <memory>

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace cg3 {
    struct check {
        virtual clang::ast_matchers::MatchFinder*
        create_finder() = 0;

        virtual void
        collected_report() { /* nop by default */ }

        virtual ~check() noexcept = default;
    };
}

#endif
