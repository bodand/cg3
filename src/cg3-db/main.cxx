/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-28.
 *
 * src/cg3-cc/main --
 */

#include <filesystem>
#include <iostream>

#include <boost/json.hpp>
#include <cg3-db/compiler_info.hxx>
#include <cg3-db/parse_dir.hxx>
#include <cg3-db/path_insert_iterator.hxx>
#include <cg3-db/path_transformer.hxx>
#include <lyra/lyra.hpp>

#include "fixup_compiler.hxx"

int
main(int argc, const char** argv) {
    bool show_help = false;
    bool show_version = false;
    bool recurse = false;
    int depth = 4;
    std::optional<cg3::compatibility> cc_type;
    std::filesystem::path cc;
    std::filesystem::path project_path = std::filesystem::current_path();
    std::vector<std::string> path_filter;
    std::vector<std::string> file_filter;
    std::vector<std::string> extra_opts;

    auto cli = lyra::cli()
               | lyra::help(show_help)("Show this help and exit")
               | lyra::opt(show_version)["-v"]["--version"]("Show version info and exit")
               | lyra::opt(recurse)["-R"]["--recurse"]("Recurse into the given directory")
               | lyra::opt(depth, "depth")["-d"]["--max-depth"]("Limit recursion depth to this (default: 4)")
               | lyra::opt([&path_filter](std::string pf) { path_filter.emplace_back(std::move(pf)); },
                           "filter")["-p"]["--path-filter"]("Filter full paths that contain this substring")
                        .cardinality(0, std::numeric_limits<std::size_t>::max())
               | lyra::opt([&file_filter](std::string ff) { file_filter.emplace_back(std::move(ff)); },
                           "filter")["-f"]["--file-filter"]("Filter filenames that contain this substring")
                        .cardinality(0, std::numeric_limits<std::size_t>::max())
               | lyra::opt([&extra_opts](std::string ff) { extra_opts.emplace_back(std::move(ff)); },
                           "option")["-O"]["--option"]("Add extra option to compiler invocation")
                        .cardinality(0, std::numeric_limits<std::size_t>::max())
               | lyra::opt([&cc_type](bool) { cc_type = cg3::compatibility::gcc; })
                      ["-G"]["--gcc"]("Set compiler compatibility to GCC")
               | lyra::opt([&cc_type](bool) { cc_type = cg3::compatibility::cl; })
                      ["-M"]["--cl"]("Set compiler compatibility to MSVC CL.EXE")
               | lyra::arg(cc, "compiler")("The compiler to use for database generation")
                        .required()
               | lyra::arg([&project_path](const std::string& pp) { project_path = pp; },
                           "path")("The directories in which to search for files (default: cwd)")
                        .optional();

    auto result = cli.parse({argc, argv});
    if (!result) {
        std::cerr << result.message() << "\n\n";
        return 2;
    }
    if (show_help) {
        std::cout << cli << "\n";
        return 1;
    }
    if (show_version) {
        std::cout << "cg3-db v1.0.0\n";
        return 0;
    }

    cg3::fixup_compiler(cc);
    if (!cc_type) cc_type = cg3::guess_compiler(cc);

    using str_it = std::string_view::const_iterator;
    std::vector<std::boyer_moore_horspool_searcher<str_it>> path_searchers;
    std::vector<std::boyer_moore_horspool_searcher<str_it>> file_searchers;

    std::transform(path_filter.begin(), path_filter.end(), std::back_inserter(path_searchers), [](std::string_view filter) {
        return std::boyer_moore_horspool_searcher(filter.cbegin(), filter.cend());
    });
    std::transform(file_filter.begin(), file_filter.end(), std::back_inserter(file_searchers), [](std::string_view filter) {
        return std::boyer_moore_horspool_searcher(filter.cbegin(), filter.cend());
    });

    std::vector<std::filesystem::path> source_files;
    if (recurse) {
        cg3::parse_recursive_dir(depth,
                                 project_path,
                                 std::back_inserter(source_files),
                                 file_searchers,
                                 path_searchers);
    }
    else {
        cg3::parse_dir(project_path,
                       std::back_inserter(source_files),
                       file_searchers,
                       path_searchers);
    }

    boost::json::array out;
    cg3::path_transformer tr(project_path, cc, *cc_type, extra_opts);
    std::copy(source_files.begin(),
              source_files.end(),
              cg3::path_inserter(out, tr));

    std::cout << out << "\n";
}
