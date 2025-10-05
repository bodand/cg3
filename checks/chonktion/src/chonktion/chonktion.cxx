/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 04.
 *
 * checks/chonktion/src/chonktion/chonktion --
 */

#include <cassert>
#include <iostream>

#include <chonktion/chonktion.hxx>

#include <clang/ASTMatchers/ASTMatchers.h>
using namespace clang::ast_matchers;

cg3::chonktion::chonktion() {
    auto big_func = functionDecl(hasBody(compoundStmt().bind("body")),
                                 isExpansionInMainFile())
                           .bind("big_func");

    _finder.addMatcher(big_func, this);
}

void
cg3::chonktion::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto&& srcmgr = *result.SourceManager;
    auto&& nodes = result.Nodes;

    auto big = nodes.getNodeAs<clang::FunctionDecl>("big_func");
    auto body = nodes.getNodeAs<clang::CompoundStmt>("body");

    auto begin_src = body->getBeginLoc();
    auto end_src = body->getEndLoc();
    auto begin = srcmgr.getCharacterData(begin_src);
    auto end = srcmgr.getCharacterData(end_src);
    auto newlines = std::count(begin, end, '\n');

    if (newlines >= gargantuan_limit) {
        handle_long_function(srcmgr, big, gargantuan_limit, 2);
    }
    else if (newlines >= huge_limit) {
        handle_long_function(srcmgr, big, huge_limit, 1);
    }
    else if (newlines >= big_limit) {
        handle_long_function(srcmgr, big, big_limit, 0);
    }
}

void
cg3::chonktion::check_ast(std::optional<boost::json::array>& json_rep,
                          std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    check::check_ast(json_rep, units);
    for (const auto& unit : units) {
        auto& ctx = unit->getASTContext();
        auto& opts = unit->getLangOpts();
        auto pp = unit->getPreprocessorPtr();
        auto& diag_engine = ctx.getDiagnostics();
        auto consumer = diag_engine.getClient();

        consumer->BeginSourceFile(opts, pp.get());

        _diag_ids[0] = diag_engine.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                                   "function %0 is a tad too big (>= %1 stmts)");
        _json_diag_ids[0] = "function `{}' is a tad too big (>= {} statements)";
        _diag_ids[1] = diag_engine.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                                   "function %0 is way too huge (>= %1 stmts)");
        _json_diag_ids[1] = "function `{}' is way too huge (>= {} statements)";
        _diag_ids[2] = diag_engine.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                                   "function %0 is inconceivably gargantuan (>= %1 stmts)");
        _json_diag_ids[2] = "function `{}' is inconceivably gargantuan (>= {} statements)";
        _finder.matchAST(ctx);

        consumer->EndSourceFile();
    }
}

namespace {
    void
    print_function_set(unsigned size, const std::vector<std::string>& funcs) {
        if (funcs.empty()) return;

        std::cout << "functions larger than " << size << " statements:\n";
        for (const auto& func : funcs) {
            std::cout << "\t" << func << "\n";
        }
        std::cout << "\n";
    }
}

void
cg3::chonktion::collected_report() {
    if (_big_funcs.empty()) return;

    std::vector<std::string> big_funcs;
    std::vector<std::string> huge_funcs;
    std::vector<std::string> gargantuan_funcs;
    for (const auto& [size, data] : _big_funcs) {
        if (size >= gargantuan_limit) {
            gargantuan_funcs.push_back(data);
        }
        else if (size >= huge_limit) {
            huge_funcs.push_back(data);
        }
        else if (size >= big_limit) {
            big_funcs.push_back(data);
        }
    }

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\nchonktion collected report\n";

    std::cout << "the following files contain the larger-than-wanted functions\n\n";
    print_function_set(big_limit, big_funcs);
    print_function_set(huge_limit, huge_funcs);
    print_function_set(gargantuan_limit, gargantuan_funcs);

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\n";
}

std::string
cg3::chonktion::json_diag_string(unsigned diag_index, std::string_view name, unsigned f_len) {
    switch (diag_index) {
    case 0:
        return std::format("function `{}' is a tad too big (>= {} statements)",
                           name,
                           f_len);
    case 1:
        return std::format("function `{}' is way too huge (>= {} statements)",
                           name,
                           f_len);
    case 2:
        return std::format("function `{}' is inconceivably gargantuan (>= {} statements)",
                           name,
                           f_len);
    default: assert(false); return "";
    }
}

void
cg3::chonktion::handle_long_function(clang::SourceManager& srcmgr,
                                     const clang::FunctionDecl* func,
                                     unsigned f_len,
                                     unsigned diag_index) {
    if (!func) return;

    auto&& diag = srcmgr.getDiagnostics();

    auto loc = func->getLocation();
    auto report = diag.Report(loc, _diag_ids[diag_index]);
    auto function_name = func->getName();
    report.AddString(function_name);
    report.AddTaggedVal(f_len, clang::DiagnosticsEngine::ak_uint);

    auto end = loc.getLocWithOffset(function_name.size());
    auto range = clang::CharSourceRange::getCharRange(loc, end);
    report.AddSourceRange(range);

    const auto fname = srcmgr.getFilename(loc);
    const auto line = srcmgr.getPresumedLineNumber(loc);
    const auto col = srcmgr.getPresumedColumnNumber(loc);
    report_json(json_diag_string(diag_index,
                                 std::string_view{function_name.data(), function_name.size()},
                                 f_len),
                fname,
                line,
                col);

    auto error_line = func->getBeginLoc().printToString(srcmgr);
    auto func_name = function_name.str();
    auto func_data = error_line + ": " + func_name;
    _big_funcs.emplace(std::piecewise_construct,
                       std::forward_as_tuple(f_len),
                       std::forward_as_tuple(func_data.data(), func_data.size()));
}
