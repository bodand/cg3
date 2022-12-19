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

#include <cg3-common/boost-json.hxx>
#include <cg3-common/filter.hxx>
#include <cg3-common/path_finder.hxx>
#include <cg3-db/compiler_info.hxx>
#include <cg3-db/fixup_compiler.hxx>
#include <cg3-db/path_insert_iterator.hxx>
#include <cg3-db/path_transformer.hxx>
#include <lyra/lyra.hpp>

int
main(int argc, const char** argv) {
    bool show_help = false;
    bool show_version = false;
    bool recurse = false;
    int depth = 4;
    std::optional<cg3::compatibility> cc_type;
    std::filesystem::path cc;
    std::filesystem::path project_path = std::filesystem::current_path();
    std::vector<std::unique_ptr<cg3::filter>> filters;
    std::vector<std::string> extra_opts;

    auto cli = lyra::cli()
               | lyra::help(show_help)("Show this help and exit")
               | lyra::opt(show_version)["-v"]["--version"]("Show version info and exit")
               | lyra::opt(recurse)["-R"]["--recurse"]("Recurse into the given directory")
               | lyra::opt(depth, "depth")["-d"]["--max-depth"]("Limit recursion depth to this (default: 4)")
               | lyra::opt([&filters](std::string pf) { filters.push_back(cg3::filter::exclude_path(pf)); },
                           "filter")["-p"]["--path-filter"]("Filter full paths that contain this substring")
                        .cardinality(0, std::numeric_limits<std::size_t>::max())
               | lyra::opt([&filters](std::string ff) { filters.push_back(cg3::filter::exclude_file(ff)); },
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
        std::cout << "cg3-db " CG3_VERSION_STRING "\n";
        return 0;
    }

    cg3::fixup_compiler(cc);
    if (!cc_type) cc_type = cg3::guess_compiler(cc);

    if (!recurse) depth = 0;
    std::unordered_set<std::string> exts{".c", ".cxx", ".c++", ".C", ".cpp", ".cc"};
    filters.push_back(cg3::filter::only_extensions(exts));
    std::vector<std::filesystem::path> source_files = cg3::find_files(project_path, filters, depth);

    boost::json::array out;
    cg3::path_transformer tr(project_path, cc, *cc_type, extra_opts);
    std::copy(source_files.begin(),
              source_files.end(),
              cg3::path_inserter(out, tr));

    std::cout << out << "\n";
}
