/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2022. 12. 19.
 *
 * src/cg3-common/find_executable --
 */

#include <optional>
#include <sstream>
#include <string>

#include <cg3-common/find_executable.hxx>

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#  define NOMINMAX
#endif

#include <windows.h>

namespace {
    constexpr const auto env_var_max_length = 32767;
    std::string
    get_env_var(const std::string& name) {
        char buf[env_var_max_length];
        auto size = GetEnvironmentVariableA(name.c_str(),
                                            static_cast<char*>(buf),
                                            static_cast<DWORD>(std::size(buf)));
        return {static_cast<char*>(buf), size};
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

std::filesystem::path
cg3::find_executable(const std::filesystem::path& exe) {
    if (exe.is_absolute()) return exe;

    auto exes = get_valid_executables(exe);
    auto sys_path = get_env_var("PATH");
    for (const auto& exe : exes) {
        if (auto maybe_exe = find_exe_in_path(exe, sys_path)) {
            return *maybe_exe;
        }
    }
    return exe;
}
