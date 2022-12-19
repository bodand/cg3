/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-28.
 *
 * src/cg3-db/path_transformer --
 */
#include <utility>

#include <cg3-db/path_transformer.hxx>

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
    for (const auto& opt : opts) {
        _args.emplace_back(opt);
    }
}
