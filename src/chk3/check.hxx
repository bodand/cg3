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
#include <clang/Frontend/ChainedDiagnosticConsumer.h>

#include "diagnostics_collection.hxx"

namespace cg3 {
    struct check;
    template<cg3::check_types Check>
    struct typed_check;

    struct check_diagnostic {
        template<class... Args>
        clang::DiagnosticBuilder
        fire(const clang::SourceLocation& loc, Args&&... args) {
            auto builder = (*_diag)->Report(loc, _diag_id(*_diag));
            (builder << ... << args);
            return builder;
        }

    private:
        friend check;

        template<std::size_t N>
        explicit check_diagnostic(check_types check,
                                  diagnostics_collection** collection,
                                  clang::DiagnosticsEngine** diag,
                                  clang::DiagnosticsEngine::Level lvl,
                                  const char (&line)[N])
             : _diag(diag),
               _diag_id([check, collection, lvl, line](clang::DiagnosticsEngine* diag) {
                   auto id = diag->getCustomDiagID(lvl, line);
                   if (*collection) (*collection)->map_diagnostic_to_check(id, check);
                   return id;
               }) { }

        clang::DiagnosticsEngine** _diag;
        std::function<unsigned(clang::DiagnosticsEngine*)> _diag_id;
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

        void
        set_collection(diagnostics_collection* collection);

        virtual void
        collected_report() { /* nop by default */
        }

        virtual ~check() noexcept = default;

    protected:
        virtual void
        match_ast([[maybe_unused]] clang::ASTContext& context) = 0;

        template<check_types Check, std::size_t N>
        auto
        create_diagnostic(clang::DiagnosticsEngine::Level lvl,
                          const char (&line)[N]) {
            auto diag = check_diagnostic(Check, &_collection, &_diag, lvl, line);
            return diag;
        }

        virtual void
        register_unregistered(diagnostics_collection* collection) = 0;

    private:
        clang::DiagnosticsEngine* _diag;
        diagnostics_collection* _collection{};
    };

    template<cg3::check_types Check>
    struct typed_check : check {
        using check::check;

        template<std::size_t N>
        check_diagnostic
        register_diagnostic(clang::DiagnosticsEngine::Level lvl,
                            const char (&line)[N]) {
            return create_diagnostic<Check>(lvl, line);
        }

        template<std::size_t N>
        check_diagnostic
        register_warning(const char (&line)[N]) {
            return register_diagnostic(clang::DiagnosticsEngine::Level::Warning, line);
        }

    protected:
        void
        register_unregistered(diagnostics_collection* collection) final {
            std::for_each(_unregisted_ids.begin(), _unregisted_ids.end(), [&](const auto& id) {
                collection->map_diagnostic_to_check(id, Check);
            });
            _unregisted_ids.clear();
        }

    private:
        std::vector<unsigned> _unregisted_ids{};
    };
}

#endif
