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
 *  - Redistributions in binary form must reproduce the above copyright notice,
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
 * src/cg3/check --
 */
#ifndef CG3_CHECK_HXX
#define CG3_CHECK_HXX

#include <cassert>
#include <memory>
#include <string_view>
#include <vector>

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Frontend/ASTUnit.h>

namespace cg3 {
    struct check;

    struct check_diagnostic {
        template<class... Args>
        clang::DiagnosticBuilder
        fire(const clang::SourceLocation& loc, Args&&... args) {
            auto builder = _diag->Report(loc, _diag_id);
            (builder << ... << args);
            return builder;
        }

    private:
        friend check;

        template<std::size_t N>
        explicit check_diagnostic(clang::DiagnosticsEngine* diag,
                                  clang::DiagnosticsEngine::Level lvl,
                                  const char (&line)[N])
             : _diag(diag),
               _diag_id(diag->getCustomDiagID(lvl, line)) { }

        clang::DiagnosticsEngine* _diag;
        unsigned _diag_id;
    };

    struct check {
        explicit check(clang::DiagnosticsEngine* diag) : _diag(diag) { }

        check(const check&) = default;
        check(check&&) noexcept = default;

        check&
        operator=(const check&) = default;
        check&
        operator=(check&&) noexcept = default;

        virtual void
        check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units);

        virtual void
        collected_report() { /* nop by default */
        }

        virtual ~check() noexcept = default;

        template<std::size_t N>
        check_diagnostic
        register_diagnostic(clang::DiagnosticsEngine::Level lvl,
                            const char (&line)[N]) {
            return check_diagnostic(_diag, lvl, line);
        }

        template<std::size_t N>
        check_diagnostic
        register_warning(const char (&line)[N]) {
            return register_diagnostic(clang::DiagnosticsEngine::Level::Warning, line);
        }

    protected:
        virtual void
        match_ast([[maybe_unused]] clang::ASTContext& context) = 0;

    private:
        clang::DiagnosticsEngine* _diag;
    };
}

#endif
