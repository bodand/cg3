/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2022. 12. 19.
 *
 * src/cg3/db_cmd --
 */

#include <cassert>

#include <cg3-common/execute_process.hxx>
#include <cg3-common/find_executable.hxx>
#include <cg3/cli-with-vector-data.hxx>
#include <cg3/db_cmd.hxx>

namespace ic = info::cli;
using namespace ic::udl;

cg3::db_cmd::db_cmd() : _parser{
       'R' / "recurse"_opt >= "Recurse into the given directory" >>= recurse,
       'd' / "depth"_opt >= "Limit recursion depth to this (default: 4)" >>= depth,
       'p' / "path-filter"_opt >= "Filter full paths that contain this substring" >>= path_filters,
       'f' / "file-filter"_opt >= "Filter filenames that contain this substring" >>= file_filters,
       'O' / "option"_opt >= "Add extra option to compiler invocation" >>= extra_opts,
       'G' / "gcc"_opt >= "Set compiler compatibility to GCC" >>= gcc_flag,
       'M' / "cl"_opt >= "Set compiler compatibility to MSVC cl" >>= msvc_flag} { }

int
cg3::db_cmd::operator()(int argc, char** argv) {
    auto params = _parser["<compiler> [<path>]"](argc, argv);
    assert(!params.empty());

    auto depth_str = std::to_string(depth);

    auto db_exe = find_executable("cg3-db").string();
    std::vector<std::string_view> cmd{
           db_exe,
           "-d",
           depth_str};
    if (gcc_flag) cmd.emplace_back("-G");
    if (msvc_flag) cmd.emplace_back("-M");
    if (recurse) cmd.emplace_back("-R");

    std::vector<std::string> formatted_opts;
    formatted_opts.reserve(extra_opts.size());
for (const auto& opt : extra_opts) formatted_opts.push_back("-O" + opt);
    for (const auto& filter : file_filters) formatted_opts.push_back("-f" + filter);
    for (const auto& filter : path_filters) formatted_opts.push_back("-p" + filter);

    std::copy(formatted_opts.begin(), formatted_opts.end(), std::back_inserter(cmd));
    std::copy(std::next(params.begin()), // !params.empty() -> at least one advacement is always safe
              params.end(),
              std::back_inserter(cmd));

    return execute_process(cmd);
}
