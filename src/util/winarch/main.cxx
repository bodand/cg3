/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2022. 12. 15.
 *
 * src/util/winarch/main --
 *  Main entry to the winarch utility program.
 *  This is used to generate a string which is a generous guess at which
 *  architecture we are running on.
 *  On Windows, this is rather limited; just x86, x86_64, aarch64, with x86 going
 *  out of support with Windows 11.
 */

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#  define NOMINMAX
#endif

// NOLINTNEXTLINE WinAPI
#define _WIN32_WINNT 0x0A00

#include <iostream>
#include <string_view>
#include <unordered_map>

#include <windows.h>

#include <wow64apiset.h>

using namespace std::literals;

int
main() {
    std::unordered_map<USHORT, std::string_view> mapping{
           {IMAGE_FILE_MACHINE_I386, "x86"sv},
           {IMAGE_FILE_MACHINE_AMD64, "x86_64"sv},
           {IMAGE_FILE_MACHINE_ARM, "arm"sv},
           {IMAGE_FILE_MACHINE_ARM64, "aarch64"sv},
           {IMAGE_FILE_MACHINE_IA64, "itanium"sv},
           {IMAGE_FILE_MACHINE_UNKNOWN, "x86"sv}, // fallback?
    };

    auto* handle = GetCurrentProcess();
    USHORT process = IMAGE_FILE_MACHINE_UNKNOWN;
    USHORT native = IMAGE_FILE_MACHINE_UNKNOWN;

    auto succ = IsWow64Process2(handle,
                                &process,
                                &native);
    if (succ == 0) {
        native = IMAGE_FILE_MACHINE_UNKNOWN;
    }

    if (auto it = mapping.find(native);
        it != mapping.end()) {
        auto&& [key, arch] = *it;
        std::cout << arch;
    }
    else {
        std::cerr << "error while trying to figure out architecture\n";
        return -1;
    }
}
