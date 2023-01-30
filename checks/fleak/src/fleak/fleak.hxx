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
 * Originally created: 2022-11-26.
 *
 * checks/fleak/src/fleak/fleak --
 */
#ifndef CG3_FLEAK_HXX
#define CG3_FLEAK_HXX

#include <algorithm>
#include <filesystem>
#include <string>

#include <cg3-common/hash_storages.hxx>
#include <chk3/check.hxx>
#include <chk3/loader.hxx>
#include <fleak/func_data.hxx>

#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace cg3 {

    struct fleak final : typed_check<check_types::fleak>,
                         clang::ast_matchers::MatchFinder::MatchCallback {
        explicit fleak(clang::DiagnosticsEngine* diag)
             : typed_check<check_types::fleak>(diag),
               _surely_leaking_diag(register_warning("function %0 opening FILE* without ever closing it")),
               _calling_note_diag(register_diagnostic(clang::DiagnosticsEngine::Note,
                                                      "called function %0 calls %1")) { }

        void
        run(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;

        void
        collected_report() override;

        void
        check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) override;

    protected:
        void
        match_ast(clang::ASTContext& context) override;

    private:
        std::unique_ptr<clang::ast_matchers::MatchFinder> _finder;
        check_diagnostic _surely_leaking_diag;
        check_diagnostic _calling_note_diag;
        std::unordered_set<func_data> _sinks{func_data("fclose", "<libc>", 0, 0)};
        std::unordered_set<func_data> _sources{func_data("fopen", "<libc>", 0, 0)};
        std::unordered_set<func_data> _leaking{};
    };

    template<>
    struct loader<check_types::fleak> {
        using type = fleak;
    };
}


#endif
