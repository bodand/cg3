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

cg3::fio::fio() {
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

cg3::fio::io_routine::operator bool() const noexcept {
    // io_routine is true if anything happened to it
    return !(opened == 0
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
cg3::fio::collected_report() {
    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\nfio collected report\n";

    if (!success_report()) {
        failed_report();
    }

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\n";
}

void
cg3::fio::failed_report() const {
    std::cout << "could not find any file io function calls in project\n";
    std::cout << "tried the following functions:\n";
    for (const auto& io : _call_data) {
        std::cout << "\t" << io.opener << ", " << io.closer << "\n";
    }
    std::cout << "\n";
}

bool
cg3::fio::success_report() const {
    bool succ = false;
    for (const auto& io : _call_data) {
        if (!io) continue;
        succ = true; // at least one thing happened

        open_close_stat(io, &io_routine::opener, &io_routine::opened, &io_routine::opened_in);
        open_close_stat(io, &io_routine::closer, &io_routine::closed, &io_routine::closed_in);
        std::cout << "\tio operations (" << io.input_called << " in/" << io.output_called << " out) using this interface took place in:\n";
        for (const auto& [call, file] : io.io_calls) {
            std::cout << "\t\t" << file << ": " << call << "\n";
        }
        std::cout << "\n";
    }
    return succ;
}

void
cg3::fio::open_close_stat(const cg3::fio::io_routine& io,
                          const std::string io_routine::*io_type,
                          int io_routine::*invoked,
                          std::vector<call_pos> io_routine::*call_files) {
    std::cout << "\t" << io.*io_type << " called " << io.*invoked << " times in the following files:\n";
    for (const auto& open_call : io.*call_files) {
        std::cout << "\t\t" << open_call << "\n";
    }
}

void
cg3::fio::check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    for (auto& unit : units) {
        auto& ctx = unit->getASTContext();
        _finder.matchAST(ctx);
    }
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
