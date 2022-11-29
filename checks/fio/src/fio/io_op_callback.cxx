/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-27.
 *
 * checks/fio/src/fio/io_op_call --
 */

#include <iostream>

#include <fio/fio.hxx>
#include <fio/io_op_callback.hxx>

cg3::io_op_callback::io_op_callback(cg3::fio* fio) : _fio(fio) { }

void
cg3::io_op_callback::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto callexpr = result.Nodes.getNodeAs<clang::CallExpr>("callexpr");
    auto call_loc = callexpr->getBeginLoc();
    auto call_file = result.SourceManager->getFilename(call_loc);
    auto call_row = result.SourceManager->getPresumedLineNumber(call_loc);
    auto call_col = result.SourceManager->getPresumedColumnNumber(call_loc);

    if (auto io_op = result.Nodes.getNodeAs<clang::FunctionDecl>("io_op");
        io_op != nullptr) {
        auto io_op_name = io_op->getName();
        std::cout << "io_opt: " << io_op_name.operator std::string_view() << "\n";

        _fio->add_io_call(io_op_name, call_file, call_row, call_col);
    }
    if (auto closer = result.Nodes.getNodeAs<clang::FunctionDecl>("closer");
        closer != nullptr) {
        auto closer_name = closer->getName();

        _fio->add_closer_call(closer_name, call_file, call_row, call_col);
    }
    if (auto opener = result.Nodes.getNodeAs<clang::FunctionDecl>("opener");
        opener != nullptr) {
        auto opener_name = opener->getName();
        if (opener_name == "CreateFileA"
            || opener_name == "CreateFileW") {
            opener_name = "CreateFile";
        }

        _fio->add_opener_call(opener_name, call_file, call_row, call_col);
    }
}
