/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-27.
 *
 * checks/fio/src/fio/fio --
 */
#include <filesystem>
#include <iostream>
#include <numeric>

#include <fio/fio.hxx>

namespace fs = std::filesystem;

using namespace clang::ast_matchers;

cg3::fio::fio(clang::DiagnosticsEngine* diag)
     : typed_check<check_types::fio>(diag) {
    auto io_calls = callExpr(eachOf(
                                    callee(functionDecl(hasName("fopen")).bind("opener")),
                                    callee(functionDecl(hasName("open")).bind("opener")),
                                    callee(functionDecl(hasName("CreateFileA")).bind("opener")),
                                    callee(functionDecl(hasName("CreateFileW")).bind("opener")),
                                    callee(functionDecl(hasName("fclose")).bind("closer")),
                                    callee(functionDecl(hasName("close")).bind("closer")),
                                    callee(functionDecl(hasName("CloseHandle")).bind("closer")),
                                    callee(functionDecl(hasName("fputs")).bind("io_op")),
                                    callee(functionDecl(hasName("fputc")).bind("io_op")),
                                    callee(functionDecl(hasName("fprintf")).bind("io_op")),
                                    callee(functionDecl(hasName("vfprintf")).bind("io_op")),
                                    callee(functionDecl(hasName("fwrite")).bind("io_op")),
                                    callee(functionDecl(hasName("fgets")).bind("io_op")),
                                    callee(functionDecl(hasName("fgetc")).bind("io_op")),
                                    callee(functionDecl(hasName("fscanf")).bind("io_op")),
                                    callee(functionDecl(hasName("vfscanf")).bind("io_op")),
                                    callee(functionDecl(hasName("fread")).bind("io_op")),
                                    callee(functionDecl(hasName("write")).bind("io_op")),
                                    callee(functionDecl(hasName("pwrite")).bind("io_op")),
                                    callee(functionDecl(hasName("writev")).bind("io_op")),
                                    callee(functionDecl(hasName("pwritev")).bind("io_op")),
                                    callee(functionDecl(hasName("read")).bind("io_op")),
                                    callee(functionDecl(hasName("pread")).bind("io_op")),
                                    callee(functionDecl(hasName("readv")).bind("io_op")),
                                    callee(functionDecl(hasName("preadv")).bind("io_op")),
                                    callee(functionDecl(hasName("WriteFile")).bind("io_op")),
                                    callee(functionDecl(hasName("WriteFileEx")).bind("io_op")),
                                    callee(functionDecl(hasName("WriteFileGather")).bind("io_op")),
                                    callee(functionDecl(hasName("ReadFile")).bind("io_op")),
                                    callee(functionDecl(hasName("ReadFileEx")).bind("io_op")),
                                    callee(functionDecl(hasName("ReadFileScatter")).bind("io_op"))),
                             isExpansionInMainFile())
                           .bind("callexpr");

    _finder.addMatcher(io_calls, &_io_op_callback);
}

[[gnu::pure]] cg3::fio::io_routine::operator bool() const noexcept {
    // io_routine is true if anything happened to it
    return !(opened == 0 // NOLINT imo no, it is not simpler by applying De Morgan's law
             && closed == 0
             && input_called == 0
             && output_called == 0);
}

void
cg3::fio::add_io_call(std::string_view io_call, std::string_view file, unsigned row, unsigned col) {
    auto it = std::find_if(_call_data.begin(), _call_data.end(), [&io_call](const auto& data) {
        return data.io_function_of(io_call);
    });
    if (it == _call_data.end()) {
        std::cerr << "cg3: fio: internal error: cannot handle call '" << io_call << "' as an IO function\n";
        return;
    }

    it->add_call_in_file(io_call, {fs::path(file), row, col});
}

void
cg3::fio::add_opener_call(std::string_view opener, std::string_view file, unsigned row, unsigned col) {
    auto it = std::find_if(_call_data.begin(), _call_data.end(), [&opener](const auto& data) {
        return data.opens_with(opener);
    });
    if (it == _call_data.end()) {
        std::cerr << "cg3: fio: internal error: cannot handle call '" << opener << "' as an IO opener\n";
        return;
    }

    it->add_open_in_file({fs::path(file), row, col});
}

void
cg3::fio::add_closer_call(std::string_view closer, std::string_view file, unsigned row, unsigned col) {
    auto it = std::find_if(_call_data.begin(), _call_data.end(), [&closer](const auto& data) {
        return data.closes_with(closer);
    });
    if (it == _call_data.end()) {
        std::cerr << "cg3: fio: internal error: cannot handle call '" << closer << "' as an IO closer\n";
        return;
    }

    it->add_close_in_file({fs::path(file), row, col});
}

void
cg3::fio::match_ast(clang::ASTContext& context) {
    _finder.matchAST(context);
}

bool
cg3::fio::io_routine::opens_with(std::string_view fun) const {
    return opener == fun;
}

bool
cg3::fio::io_routine::closes_with(std::string_view fun) const {
    return closer == fun;
}

bool
cg3::fio::io_routine::io_function_of(std::string_view fun) const {
    auto it = std::find(io_functions.begin(),
                        io_functions.end(),
                        fun);
    return it != io_functions.end();
}

void
cg3::fio::io_routine::add_open_in_file(call_pos&& file) {
    ++opened;
    opened_in.emplace_back(std::move(file));
}

void
cg3::fio::io_routine::add_close_in_file(call_pos&& file) {
    ++closed;
    closed_in.emplace_back(std::move(file));
}

void
cg3::fio::io_routine::add_call_in_file(std::string_view call, call_pos&& file) {
    auto it = std::find(io_functions.begin(),
                        io_functions.end(),
                        call);
    if (it == io_functions.end()) {
        assert(false); // should not happen
        return;
    }

    io_calls.emplace(std::piecewise_construct,
                     std::forward_as_tuple(call.data(), call.size()),
                     std::forward_as_tuple(std::move(file)));
    if (it->out) {
        ++output_called;
    }
    else {
        ++input_called;
    }
}
