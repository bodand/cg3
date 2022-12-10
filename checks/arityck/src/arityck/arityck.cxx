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
 * checks/arityck/src/arityck/arityck --
 */

#include <iostream>

#include <arityck/arityck.hxx>

#include <clang/ASTMatchers/ASTMatchers.h>
using namespace clang::ast_matchers;

namespace {
    template<class Fn>
    struct parameter_comparator_interface final : internal::SingleNodeMatcherInterface<clang::FunctionDecl> {
        explicit parameter_comparator_interface(Fn&& f)
             : _fn(f) { }

        bool
        matchesNode(const clang::FunctionDecl& node) const override {
            auto param_cnt = node.getNumParams();
            return _fn(param_cnt);
        }

    private:
        Fn _fn{};
    };

    struct parameter_comparator final : internal::Matcher<clang::FunctionDecl> {
        explicit parameter_comparator(internal::MatcherInterface<clang::FunctionDecl>* implementation)
             : Matcher(implementation) { }
    };

    template<class Fn>
    parameter_comparator
    parameter_count_is(Fn&& fn) {
        return parameter_comparator{
               new parameter_comparator_interface<Fn>(std::forward<Fn>(fn))};
    }
}

cg3::arityck::arityck() {
    auto check = functionDecl(parameter_count_is([](unsigned cnt) {
                                  return cnt >= 5;
                              }),
                              isExpansionInMainFile())
                        .bind("sus_function");

    _finder.addMatcher(check, this);
}

void
cg3::arityck::collected_report() {
    if (_high_arity_funcs.empty()) return;

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\narityck collected report\n";

    std::cout << "the following files contain functions which have a lot of parameters\n\n";
    for (const auto& [pos, func] : _high_arity_funcs) {
        std::cout << "\t" << pos << ": " << func << "\n";
    }
    std::cout << "\n";

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\n";
}

void
cg3::arityck::check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    for (const auto& unit : units) {
        auto& ctx = unit->getASTContext();
        auto& opts = unit->getLangOpts();
        auto pp = unit->getPreprocessorPtr();
        auto& diag_engine = ctx.getDiagnostics();
        auto consumer = diag_engine.getClient();

        consumer->BeginSourceFile(opts, pp.get());

        _diag_id = diag_engine.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                               "function %0 with suspiciously many parameters (%1)");
        _finder.matchAST(ctx);

        consumer->EndSourceFile();
    }
}

void
cg3::arityck::run(const MatchFinder::MatchResult& result) {
    auto&& srcmgr = *result.SourceManager;
    auto&& diag = result.Context->getDiagnostics();

    auto sus_fn = result.Nodes.getNodeAs<clang::FunctionDecl>("sus_function");
    auto loc = sus_fn->getLocation();

    auto builder = diag.Report(loc, _diag_id);
    builder.AddString(sus_fn->getName());
    builder.AddTaggedVal(sus_fn->param_size(),
                         clang::DiagnosticsEngine::ak_uint);

    auto end = loc.getLocWithOffset(sus_fn->getName().size());
    auto range = clang::CharSourceRange::getCharRange(loc, end);
    builder.AddSourceRange(range);

    auto fn_begin = sus_fn->getSourceRange().getBegin();
    _high_arity_funcs.emplace(fn_begin.printToString(srcmgr),
                              sus_fn->getName().str());
}
