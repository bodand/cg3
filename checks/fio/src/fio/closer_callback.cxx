/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-27.
 *
 * checks/fio/src/fio/closer_callback --
 */

#include <fio/closer_callback.hxx>
#include <fio/fio.hxx>

cg3::closer_callback::closer_callback(cg3::fio* fio) : _fio(fio) { }

void
cg3::closer_callback::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto callexpr = result.Nodes.getNodeAs<clang::CallExpr>("callexpr");
    auto call_loc = callexpr->getBeginLoc();
    auto call_file = result.SourceManager->getFilename(call_loc);

    auto closer = result.Nodes.getNodeAs<clang::FunctionDecl>("closer");
    auto closer_name = closer->getName();

    _fio->add_closer_call(closer_name, call_file);
}
