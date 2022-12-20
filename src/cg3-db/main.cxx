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
#include <info/cli/types/type_data.hxx>
template<class T>
struct info::cli::type_data<std::vector<T>> {
    constexpr const static bool allow_nothing = type_data<T>::allow_nothing;
    constexpr const static std::string_view default_value = type_data<T>::default_value;
    constexpr const static int length = type_data<T>::length;
    constexpr const static std::string_view type_name = meta::type_name<T>();
    constexpr const static parse_type expected_type = type_data<T>::expected_type;
};

#include <info/cli.hxx>
using namespace info::cli::udl;
namespace cli = info::cli;

int
main(int argc, char** argv) {
    bool show_version = false;
    bool recurse = false;
    int depth = 4;
    std::optional<cg3::compatibility> cc_type;
    std::filesystem::path cc;
    std::filesystem::path project_path = std::filesystem::current_path();
    std::vector<std::unique_ptr<cg3::filter>> filters;
    std::vector<std::string> extra_opts;

    // clang-format off
    cli::cli_parser cli{
           'v' / "version"_opt >= "Show version info and exit" >>= show_version,
           'R' / "recurse"_opt >= "Recurse into the given directory" >>= recurse,
           'd' / "depth"_opt >= "Limit recursion depth to this (default: 4)" >>= depth,
           'p' / "path-filter"_opt >= "Filter full paths that contain this substring"
                    >>= [&filters](std::string_view str) { filters.push_back(cg3::filter::exclude_path(str)); },
           'f' / "file-filter"_opt >= "Filter full files that contain this substring"
                    >>= [&filters](std::string_view str) { filters.push_back(cg3::filter::exclude_file(str)); },
           'O' / "option"_opt >= "Add extra option to compiler invocations" >>= extra_opts,
           'G' / "gcc"_opt >= "Set compiler compatibility to GCC"
                    >>= [&cc_type](bool val) { if (val) cc_type = cg3::compatibility::gcc; },
           'M' / "cl"_opt >= "Set compiler compatibility to MSVC/CL.EXE"
                    >>= [&cc_type](bool val) { if (val) cc_type = cg3::compatibility::gcc; }
    };
    // clang-format on

    try {
        auto params = cli["<compiler> [<path>]"](argc, argv);

        if (show_version) {
            std::cout << "cg3-db " CG3_VERSION_STRING "\n";
            return 0;
        }

        if (params.size() < 2) throw std::runtime_error("expected <compiler>");
        if (params.size() > 3) throw std::runtime_error("unrecognized parameters passed");

        cc = params[1];
        if (params.size() > 2) project_path = params[2];

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << "\n";
        return 2;
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
