/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-29.
 *
 * src/cg3-db/fixup_compiler --
 */

#include <optional>
#include <sstream>
#include <string>

#include <cg3-db/fixup_compiler.hxx>

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#  define NOMINMAX
#endif

#include <windows.h>

namespace {
    std::string
    get_env_var(const std::string& name) {
        char buf[32767];
        auto size = GetEnvironmentVariableA(name.c_str(),
                                            buf,
                                            static_cast<DWORD>(std::size(buf)));
        return {buf, size};
    }

    std::vector<std::filesystem::path>
    get_valid_executables(const std::filesystem::path& stem) {
        std::vector<std::filesystem::path> ret;
        if (stem.has_extension()) {
            // if stem has extensions, we don't try to guess which extension
            // it has; just return it as-is
            ret.emplace_back(stem);
            return ret;
        }

        auto pathext = std::istringstream{get_env_var("PATHEXT")};
        std::string ext;
        while (std::getline(pathext, ext, ';')) {
            auto new_path = stem;
            new_path += ext;
            ret.push_back(new_path);
        }

        return ret;
    }

    std::optional<std::filesystem::path>
    find_exe_in_path(const std::filesystem::path& exe,
                     const std::string& sys_path) {
        auto pathext = std::istringstream{sys_path};
        std::string ext;
        while (std::getline(pathext, ext, ';')) {
            auto exe_path = std::filesystem::path{ext} / exe;
            if (exists(exe_path)
                && is_regular_file(exe_path)) {
                return exe_path;
            }
        }

        return std::nullopt;
    }
}

void
cg3::fixup_compiler(std::filesystem::path& cc) {
    if (cc.is_absolute()) return;

    auto exes = get_valid_executables(cc);
    auto sys_path = get_env_var("PATH");
    for (const auto& exe : exes) {
        if (auto maybe_cc = find_exe_in_path(exe, sys_path)) {
            cc = *maybe_cc;
            return;
        }
    }
}
