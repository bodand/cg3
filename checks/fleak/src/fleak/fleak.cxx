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
 * checks/globus/src/globus/globus --
 */

#include <algorithm>
#include <filesystem>
#include <iostream>

#include <fleak/fleak.hxx>
#include <fleak/func_data.hxx>

#include <clang/ASTMatchers/ASTMatchers.h>

using namespace clang::ast_matchers;
namespace fs = std::filesystem;

namespace {
    struct callset_matcher_interface : internal::SingleNodeMatcherInterface<clang::FunctionDecl> {
        callset_matcher_interface(const std::unordered_set<cg3::func_data>& funcs)
             : functions() {
            std::transform(funcs.begin(),
                           funcs.end(),
                           std::inserter(functions, functions.begin()),
                           [](const auto& func) {
                               return func.name;
                           });
        }

        bool
        matchesNode(const clang::FunctionDecl& node) const override {
            auto it = functions.find(node.getNameAsString());
            return it != functions.end();
        }

        std::unordered_set<std::string> functions;
    };

    struct callset_matcher : internal::Matcher<clang::FunctionDecl> {
        explicit callset_matcher(internal::MatcherInterface<clang::FunctionDecl>* implementation)
             : Matcher(implementation) { }
    };

    callset_matcher
    any_name_of(const std::unordered_set<cg3::func_data>& funcs) {
        return callset_matcher(new callset_matcher_interface(funcs));
    }
}

cg3::fleak::fleak() = default;

void
cg3::fleak::check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    auto file_ptr = pointsTo(qualType(asString("FILE")));
    auto file_ptr_out = pointsTo(file_ptr);
    auto file_source = anyOf(returns(file_ptr),
                             hasAnyParameter(hasType(file_ptr_out)));
    auto file_sink = hasAnyParameter(hasType(file_ptr));
    auto calls_function = [](std::string_view fun) {
        return hasBody(hasDescendant(callExpr(
                                            callee(functionDecl(hasName(fun)).bind("called_function")))
                                            .bind("called_at")));
    };

    bool more;
    do {
        more = false;
        MatchFinder finder;
        for (auto& source : _sources) {
            if (!source.get_checked()) {
                source.set_checked();
                more = true;
                auto matcher = functionDecl(file_source, calls_function(source.name))
                                      .bind("file_source");
                finder.addMatcher(matcher, this);
            }
        }
        for (auto& sink : _sinks) {
            if (!sink.get_checked()) {
                sink.set_checked();
                more = true;
                auto matcher = functionDecl(file_sink, calls_function(sink.name))
                                      .bind("file_sink");
                finder.addMatcher(matcher, this);
            }
        }

        for (const auto& unit : units) {
            finder.matchAST(unit->getASTContext());
        }
    } while (more);

    MatchFinder leak_finder;
    auto calls_source = hasBody(hasDescendant(callExpr(
                                                     callee(functionDecl(any_name_of(_sources)).bind("called_source")))
                                                     .bind("called_at")));
    auto calls_sink = hasBody(hasDescendant(callExpr(callee(functionDecl(any_name_of(_sinks))))));
    auto not_source = unless(any_name_of(_sources));
    auto not_sink = unless(any_name_of(_sinks));

    auto check = functionDecl(not_sink, not_source, calls_source, unless(calls_sink)).bind("file_leak");
    leak_finder.addMatcher(check, this);
    for (const auto& unit : units) {
        leak_finder.matchAST(unit->getASTContext());
    }
}

namespace {
    auto
    find_func(const std::unordered_set<cg3::func_data>& data,
              const clang::FunctionDecl* called) {
        auto it = std::find_if(data.begin(), data.end(), [&called](const auto& func) {
            return called->getName() == func;
        });
        return it;
    }

    const cg3::func_data*
    get_called_function_or_null(const std::unordered_set<cg3::func_data>& data,
                                const clang::FunctionDecl* called) {
        auto it = find_func(data, called);
        if (it == data.end()) return nullptr;
        return std::addressof(*it);
    }

    void
    insert_if_not_null(const clang::FunctionDecl* func,
                       const clang::FunctionDecl* called,
                       const clang::CallExpr* called_at,
                       std::unordered_set<cg3::func_data>& data,
                       const clang::SourceManager& srcmgr) {
        if (!func) return;
        auto called_ptr = get_called_function_or_null(data,
                                                      called);
        auto called_src_range = called_at->getSourceRange();
        auto called_src = called_src_range.getBegin();

        auto fname = srcmgr.getFilename(called_src);
        auto row = srcmgr.getPresumedLineNumber(called_src);
        auto col = srcmgr.getPresumedColumnNumber(called_src);

        data.emplace(func->getName(),
                     fs::path(fname.str()),
                     row,
                     col,
                     called_ptr);
    }
}

void
cg3::fleak::run(const MatchFinder::MatchResult& result) {
    auto&& nodes = result.Nodes;
    auto&& srcmgr = *result.SourceManager;

    auto called = nodes.getNodeAs<clang::FunctionDecl>("called_function");
    auto called_at = nodes.getNodeAs<clang::CallExpr>("called_at");

    auto sink = nodes.getNodeAs<clang::FunctionDecl>("file_sink");
    insert_if_not_null(sink, called, called_at, _sinks, srcmgr);

    auto source = nodes.getNodeAs<clang::FunctionDecl>("file_source");
    insert_if_not_null(source, called, called_at, _sources, srcmgr);

    if (auto leak = nodes.getNodeAs<clang::FunctionDecl>("file_leak")) {
        auto leaking_rng = leak->getSourceRange();
        auto leaking_begin = leaking_rng.getBegin();

        auto leaking_file = srcmgr.getFilename(leaking_begin);
        auto leaking_row = srcmgr.getPresumedLineNumber(leaking_begin);
        auto leaking_col = srcmgr.getPresumedColumnNumber(leaking_begin);

        _leaking.emplace(leak->getName(),
                         fs::path(leaking_file.str()),
                         leaking_row,
                         leaking_col);

        // called_at valid
        auto called_source = nodes.getNodeAs<clang::FunctionDecl>("called_source");
        auto source_func = find_func(_sources, called_source);

        const func_data* func = std::addressof(*source_func);

        auto called_rng = called_at->getSourceRange();
        auto call_begin = called_rng.getBegin();
        auto call_end = called_rng.getEnd();

        auto error_line = call_begin.printToString(srcmgr);

        auto begin = srcmgr.getCharacterData(call_begin);
        auto end = srcmgr.getCharacterData(call_end);

        std::cout << error_line << ": fleak: error: function producing FILE* without ever closing it:\n\t";
        std::cout << leak->getName().str() << " calls " << func->name << ":\n\n\t";
        std::copy(begin, end + 1, std::ostream_iterator<char>(std::cout));
        std::cout << "\n\t^";
        std::fill_n(std::ostream_iterator<char>(std::cout), func->name.size() - 1, '~');
        std::cout << "\n";

        while (func->calls) {
            std::cout << func->file.string() << ":" << func->row << ":" << func->col;
            func = func->calls;
            std::cout << ": fleak: note: called function calls " << func->name << "\n";
        }
    }
}

void
cg3::fleak::collected_report() {
    if (_leaking.empty()) return;

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\nfleak collected report\n";
    std::cout << "\n";

    std::cout << "the following functions are likely to leak FILE handles: \n";
    for (const auto& func : _leaking) {
        std::cout << "\t" << func.file.string() << ":" << func.row << ":" << func.col << ": " << func.name << "\n";
    }
    std::cout << "\n";

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\n";
}
