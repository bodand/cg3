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
#include <unordered_map>

#include <chk3/check.hxx>
#include <chk3/loader.hxx>

#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace cg3 {
    struct chonktion final : check,
                             clang::ast_matchers::MatchFinder::MatchCallback {
        static constexpr auto gargantuan_limit = 256u;
        static constexpr auto huge_limit = 128u;
        static constexpr auto big_limit = 64u;

        chonktion();

        void
        run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

        void
        check_ast(std::optional<boost::json::array>& json_rep,
                  std::vector<std::unique_ptr<clang::ASTUnit>>& units) override;

        void
        collected_report() override;

        std::string_view
        check_name() const noexcept override { return "chonktion"; }

    private:
        unsigned _diag_ids[3];
        std::string_view _json_diag_ids[3];
        std::unordered_multimap<unsigned, std::string> _big_funcs{};
        clang::ast_matchers::MatchFinder _finder{};

        static std::string
        json_diag_string(unsigned diag_index, std::string_view name, unsigned f_len);

        void
        handle_long_function(clang::SourceManager& srcmgr,
                             const clang::FunctionDecl* func,
                             unsigned f_len,
                             unsigned diag_index);
    };

    template<>
    struct loader<check_types::chonktion> {
        using type = chonktion;
    };
}

#endif
