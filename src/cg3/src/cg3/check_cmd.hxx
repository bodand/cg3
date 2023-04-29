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

#ifndef CG3_CHECK_CMD_HXX
#define CG3_CHECK_CMD_HXX

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <cg3-common/filter.hxx>
#include <cg3-common/hash_storages.hxx>
#include <cg3/cli-with-vector-data.hxx>

namespace cg3 {
    struct check_cmd {
        check_cmd();

        int
        operator()(int argc, char** argv);

    private:
        const std::unordered_set<std::string> _source_extensions{".c", ".cxx", ".c++", ".C", ".cpp", ".cc"};
        // clang-format off
        const std::unordered_set<std::string> _all_extensions{".c", ".cxx", ".c++", ".C", ".cpp", ".cc",
                                                              ".h", ".hxx", ".h++", ".H", ".hpp", ".hh"};
        // clang-format on
        std::string _compiler{};
        std::vector<std::unique_ptr<cg3::filter>> _filters;
        bool _recurse = false;
        int _depth = 4;
        info::cli::cli_parser _parser;
    };
}

#endif
