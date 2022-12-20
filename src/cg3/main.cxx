/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2022. 12. 19.
 *
 * src/cg3/main --
 */

#include <cassert>
#include <iostream>

#include <cg3/db_cmd.hxx>
#include <cg3/check_cmd.hxx>

[[noreturn]] void
usage() {
    std::cout << "USAGE:\n"
                 "cg3 check [-dfhMpR] [LONG_OPTIONS] (<directory> | <source>)... \n"
                 "cg3 db [-dfGhMOpR] [LONG_OPTIONS] <compiler> [<path>]\n"
                 "cg3 [-hv] [LONG_OPTIONS]\n"
                 "\n"
                 "Options:\n"
                 "\t-h, --help\n"
                 "\t\tShows help and exits\n"
                 "\t-v, --version\n"
                 "\t\tShows version information and exits\n";
    std::exit(1);
}

int
main(int argc, char** argv) {
    if (argc == 1) usage();
    assert(argc >= 2);

    cg3::db_cmd db;
    cg3::check_cmd check;

    std::string_view cmd = argv[1];
    if (cmd == "db")
        return db(argc - 1, argv + 1);
    else if (cmd == "check") {
        return check(argc - 1, argv + 1);
    }
    else if (cmd == "-v" || cmd == "--version") {
        std::cout << "cg3 " CG3_VERSION_STRING "\n";
    }
    else {
        usage();
    }
}
