/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 04.
 *
 * checks/chonktion/src/chonktion/chonktion --
 */

#ifndef CG3_CHONKTION_HXX
#define CG3_CHONKTION_HXX

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include <cg3-common/hash_storages.hxx>
#include <chk3/check.hxx>
#include <chk3/loader.hxx>

#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace cg3 {
    struct chonktion final : check,
                             clang::ast_matchers::MatchFinder::MatchCallback {
        static constexpr auto gargantuan_limit = 256U;
        static constexpr auto huge_limit = 128U;
        static constexpr auto big_limit = 64U;

        explicit chonktion(clang::DiagnosticsEngine* diag);

        void
        run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

        void
        collected_report() override;

    protected:
        void
        match_ast(clang::ASTContext& context) override;

    private:
        check_diagnostic _big_diag;
        check_diagnostic _huge_diag;
        check_diagnostic _gargantuan_diag;
        std::unordered_multimap<unsigned, std::string> _big_funcs{};
        clang::ast_matchers::MatchFinder _finder{};

        void
        handle_long_function(clang::SourceManager& srcmgr,
                             const clang::FunctionDecl* func,
                             unsigned f_len,
                             check_diagnostic& diag_h);
    };

    template<>
    struct loader<check_types::chonktion> {
        using type = chonktion;
    };
}

#endif
