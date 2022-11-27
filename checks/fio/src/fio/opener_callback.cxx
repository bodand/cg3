/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-27.
 *
 * checks/fio/src/fio/file_routine_callback --
 */

#include <fio/fio.hxx>
#include <fio/opener_callback.hxx>

cg3::opener_callback::opener_callback(cg3::fio* fio)
     : _fio(fio) { }

void
cg3::opener_callback::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto callexpr = result.Nodes.getNodeAs<clang::CallExpr>("callexpr");
    auto call_loc = callexpr->getBeginLoc();
    auto call_file = result.SourceManager->getFilename(call_loc);

    auto opener = result.Nodes.getNodeAs<clang::FunctionDecl>("opener");
    auto opener_name = opener->getName();
    if (opener_name == "CreateFileA"
        || opener_name == "CreateFileW") {
        opener_name = "CreateFile";
    }

    _fio->add_opener_call(opener_name, call_file);
}
