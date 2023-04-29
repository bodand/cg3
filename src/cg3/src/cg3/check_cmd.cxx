/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2022. 12. 20.
 *
 * src/cg3/check_cmd --
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include <cg3-common/execute_process.hxx>
#include <cg3-common/find_executable.hxx>
#include <cg3-common/path_finder.hxx>
#include <cg3/check_cmd.hxx>
#include <cg3/cli-with-vector-data.hxx>

namespace ic = info::cli;
using namespace ic::udl;

cg3::check_cmd::check_cmd()
     : _parser{
            // clang-format off
            'M' / "make-db"_opt >= "Generate compilation database before checking" >>= _compiler,
            'R' / "recurse"_opt >= "Recurse into the given directory" >>= _recurse,
            'd' / "depth"_opt >= "Limit recursion depth to this (default: 4)" >>= _depth,
            'p' / "path-filter"_opt >= "Filter full paths that contain this substring"
                >>= [&_filters = _filters](std::string_view f) { _filters.emplace_back(cg3::filter::exclude_path(f)); },
            'f' / "file-filter"_opt >= "Filter filenames that contain this substring"
                >>= [&_filters = _filters](std::string_view f) { _filters.emplace_back(cg3::filter::exclude_file(f)); }} {
    // clang-format on
    _filters.push_back(cg3::filter::only_extensions(_all_extensions));
    _parser.unknown_behavior(ic::unknown_behavior::pass_back);
}

int
cg3::check_cmd::operator()(int argc, char** argv) {
    auto args = _parser["(<directory> | <source>)..."](argc, argv);
    assert(!args.empty());
    if (!_recurse) _depth = 0;

    std::vector<std::string> files_long_live;
    for (auto arg : args) {
        auto path = std::filesystem::path(arg);
        if (is_directory(path)) {
            auto files = find_files(path, _filters, _depth);
            std::transform(files.begin(), files.end(), std::back_inserter(files_long_live), [](const auto& file) {
                return file.string();
            });
        }
    }
    args.erase(std::remove_if(args.begin(), args.end(), [](std::string_view name) {
                   auto path = std::filesystem::path(name);
                   return is_directory(path);
               }),
               args.end());
    std::copy(files_long_live.begin(), files_long_live.end(), std::back_inserter(args));

    if (!_compiler.empty()) {
        std::vector<std::string> filters;
        filters.reserve(_filters.size());
        for (auto& filter : _filters) {
            auto str = filter->to_string();
            if (!str.empty()) filters.emplace_back(std::move(str));
        }
        _filters.push_back(cg3::filter::only_extensions(_source_extensions));

        std::vector<std::string> sources_long_live;
        for (auto arg : args) {
            auto path = std::filesystem::path(arg);
            if (is_directory(path)) {
                auto files = find_files(path, _filters, _depth);
                std::transform(files.begin(), files.end(), std::back_inserter(sources_long_live), [](const auto& file) {
                    return file.string();
                });
            }
        }
        _filters.pop_back();

        auto exe = find_executable("cg3-db").string();
        auto depth_str = std::to_string(_depth);
        std::vector<std::string_view> db_args{
               exe,
               _compiler,
               "-d",
               depth_str,
        };
        if (_recurse) db_args.emplace_back("-R");
        std::copy(filters.begin(), filters.end(), std::back_inserter(db_args));
        std::copy(sources_long_live.begin(), sources_long_live.end(), std::back_inserter(db_args));

        std::istringstream input{""};
        std::ofstream output("compile_commands.json");
        auto status = execute_process(db_args, input, output, std::cerr);
        if (status != 0) return status;
    }

    auto check_exe = find_executable("cg3-check").string();
    args[0] = check_exe; // replace check command with check_exe
    return execute_process(args);
}
