/* cg3 project
 *
 * Copyright (c) 2022 András Bodor <bodand@pm.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * - Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
        bugmalloc();

        void
        check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) override;

        void
        collected_report() override;

        void
        add_invalid_file(std::string_view filename);
        void
        add_call(const std::string& fun,
                 std::string_view filename);

        void
        hijacked_call();

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
