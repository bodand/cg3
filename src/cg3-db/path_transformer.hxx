/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-28.
 *
 * src/cg3-db/path_transformer --
 */
#ifndef CG3_PATH_TRANSFORMER_HXX
#define CG3_PATH_TRANSFORMER_HXX

#include <filesystem>

#include <boost/json.hpp>
#include <cg3-db/compiler_info.hxx>

namespace cg3 {
    struct path_transformer {
        path_transformer(std::filesystem::path directory,
                         const std::filesystem::path& cc,
                         cg3::compatibility cc_type,
                         const std::vector<std::string>& opts);

        boost::json::object
        operator()(const std::filesystem::path& file) const;

        boost::json::object
        operator()(std::filesystem::path&& file) const;

    private:
        boost::json::array _args{};
        std::filesystem::path _directory;
        cg3::compatibility _cc_type;
        [[nodiscard]] boost::json::array
        get_file_args(const std::filesystem::path& rel_path) const;
    };
}

#endif
