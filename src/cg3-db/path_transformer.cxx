/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-28.
 *
 * src/cg3-db/path_transformer --
 */
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include <cg3-common/execute_process.hxx>
#include <cg3-db/path_transformer.hxx>

namespace {
    void
    add_msvc_stanard_includes(boost::json::array& args,
                              const std::filesystem::path& cc) {
        auto vroot = cc.parent_path()       // host-spec
                            .parent_path()  // bin directory
                            .parent_path(); // version root
        auto inc_dir = (vroot / "include").string();
        args.emplace_back(cg3::get_include_option(cg3::compatibility::cl));
        args.emplace_back(inc_dir);
    }

    void
    add_gcc_stanard_includes(boost::json::array& args,
                             const std::filesystem::path& cc) {
        try {
            std::istringstream in_strm{""};
            std::stringstream out_strm;
            std::stringstream err_strm;
            std::vector<std::string_view> cmd{cc.string(), "-xc", "-E", "-v", "-"};
            cg3::execute_process(cmd, in_strm, out_strm, err_strm);

            std::vector<std::filesystem::path> paths;
            // skip until we find a #, then read all lines starting with a space
            std::string line;
            bool skip = true;
            while (std::getline(err_strm, line)) {
                char front = line.front();
                if (skip) {
                    skip = front != '#';
                }
                else if (front == ' ') {
                    auto end = line.npos;
                    if (line.back() == '\r') {
                        end = line.size() - 2;
                    }
                    paths.emplace_back(line.substr(1, end));
                }
            }

            for (auto&& path : paths) {
                args.emplace_back(cg3::get_include_option(cg3::compatibility::gcc));
                args.emplace_back(path.string());
            }
        } catch (const std::exception& ex) {
            std::cerr << "error executing gcc: " << ex.what() << "\n";
        }
    }

    void
    add_compiler_standard_includes(boost::json::array& args,
                                   cg3::compatibility cc_type,
                                   const std::filesystem::path& cc) {
        switch (cc_type) {
        case cg3::compatibility::gcc:
            add_gcc_stanard_includes(args, cc);
            return;
        case cg3::compatibility::cl:
            add_msvc_stanard_includes(args, cc);
            return;
        }
    }
}

boost::json::object
cg3::path_transformer::operator()(const std::filesystem::path& file) const {
    boost::json::object obj;
    obj["directory"] = _directory.string();
    obj["file"] = file.string();
    obj["arguments"].emplace_array() = get_file_args(file);

    return obj;
}

boost::json::object
cg3::path_transformer::operator()(std::filesystem::path&& file) const {
    auto local_file = std::move(file);

    boost::json::object obj;
    obj["directory"] = _directory.string();
    obj["file"] = local_file.string();
    obj["arguments"].emplace_array() = get_file_args(local_file);

    return obj;
}

boost::json::array
cg3::path_transformer::get_file_args(const std::filesystem::path& rel_path) const {
    auto rel_obj = rel_path;
    rel_obj.replace_extension(".o");

    auto file_args = _args;
    file_args.emplace_back(rel_path.string());
    file_args.emplace_back(get_output_name_option(_cc_type));
    file_args.emplace_back(rel_obj.string());
    return file_args;
}

cg3::path_transformer::path_transformer(std::filesystem::path directory,
                                        const std::filesystem::path& cc,
                                        cg3::compatibility cc_type,
                                        const std::vector<std::string>& opts)
     : _directory(std::move(directory)),
       _cc_type(cc_type) {
    _args.emplace_back(cc.string());
    _args.emplace_back(get_compile_flag(_cc_type));
    add_compiler_standard_includes(_args, _cc_type, cc);
    for (const auto& opt : opts) {
        _args.emplace_back(opt);
    }
}
