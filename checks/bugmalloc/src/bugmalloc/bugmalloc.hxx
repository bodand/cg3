/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-25.
 *
 * checks/bugmalloc/src/bugmalloc --
 */
#ifndef CG3_BUGMALLOC_HXX
#define CG3_BUGMALLOC_HXX

#include <filesystem>
#include <string_view>
#include <unordered_set>

#include <bugmalloc/invalid_malloc_callback.hxx>
#include <chk3/check.hxx>
#include <chk3/loader.hxx>

namespace cg3 {
    struct bugmalloc final : check {
        clang::ast_matchers::MatchFinder*
        create_finder() override;

        void
        collected_report() override;

        void
        add_invalid_file(std::string_view filename);
        void
        add_call(const std::string& fun,
                 std::string_view filename);

    private:
        bool any_called = false;
        const std::unordered_set<std::string> _standard_funcs{"malloc",
                                                              "calloc",
                                                              "realloc",
                                                              "free"};

        std::unordered_multimap<std::string, std::filesystem::path> _tricky_functions;
        std::unordered_set<std::filesystem::path> _files_to_report;
        clang::ast_matchers::MatchFinder _finder{};
        invalid_malloc_callback _malloc_callback{this};
    };

    template<>
    struct loader<check_types::bugmalloc> {
        using type = bugmalloc;
    };
}

#endif
