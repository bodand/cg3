/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-29.
 *
 * src/cg3-db/compiler_info --
 */
#ifndef CG3_COMPILER_INFO_HXX
#define CG3_COMPILER_INFO_HXX

#include <filesystem>
#include <string>

#include <boost/json/array.hpp>

namespace cg3 {
    enum class compatibility {
        gcc,
        cl
    };

    compatibility
    guess_compiler(const std::filesystem::path& cc);

    void
    put_output_name_option(compatibility cc, boost::json::array& args, const std::filesystem::path& out_file);
    std::string
    get_compile_flag(compatibility cc);
    std::string
    get_include_option(compatibility cc);
}

#endif
