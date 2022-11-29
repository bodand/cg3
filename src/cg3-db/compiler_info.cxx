/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-29.
 *
 * src/cg3-db/compiler_info --
 */
#include <algorithm>

#include <cg3-db/compiler_info.hxx>

std::string
cg3::get_output_name_option(cg3::compatibility cc) {
    switch (cc) {
    case compatibility::gcc: return "-o";
    case compatibility::cl: return "/Fo:";
    }
}

std::string
cg3::get_compile_flag(cg3::compatibility cc) {
    switch (cc) {
    case compatibility::gcc: return "-c";
    case compatibility::cl: return "/c";
    }
}

cg3::compatibility
cg3::guess_compiler(const std::filesystem::path& cc) {
    auto str = cc.filename().string();
    auto gcc = str.find("gcc");
    if (gcc != std::string::npos) return compatibility::gcc;

    auto cl = str.find("cl");
    if (cl != std::string::npos) return compatibility::cl;

    return compatibility::gcc;
}
