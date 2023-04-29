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

#ifndef CG3_DB_CMD_HXX
#define CG3_DB_CMD_HXX

#include <filesystem>
#include <string>
#include <vector>

#include <cg3/cli-with-vector-data.hxx>

namespace cg3 {
    struct db_cmd {
        db_cmd();

        int
        operator()(int argc, char** argv);

    private:
        bool gcc_flag = false;
        bool msvc_flag = false;
        bool recurse = false;
        int depth = 4;
        std::vector<std::string> extra_opts;
        std::vector<std::string> file_filters;
        std::vector<std::string> path_filters;
        info::cli::cli_parser _parser;
    };
}

#endif
