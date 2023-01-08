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
 *
 *  WARNING: This code must not use exceptions; this is because clang-cl fails without specific
 *  exception handling code set (/EHsc), which try_run refuses to pass.
 *  Easier to just exception-less code, than to try to fight with CMake even more.
 */

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#  define NOMINMAX
#endif

// NOLINTNEXTLINE WinAPI
#define _WIN32_WINNT 0x0A00

#include <cstdio>
#include <tuple>

#include <windows.h>

#include <wow64apiset.h>


int
main() {
    auto* handle = GetCurrentProcess();
    USHORT process = IMAGE_FILE_MACHINE_UNKNOWN;
    USHORT native = IMAGE_FILE_MACHINE_UNKNOWN;

    auto succ = IsWow64Process2(handle,
                                &process,
                                &native);
    if (succ == 0) {
        native = IMAGE_FILE_MACHINE_UNKNOWN;
    }

    switch (native) {
    case IMAGE_FILE_MACHINE_I386:
        std::puts("x86");
        break;
    case IMAGE_FILE_MACHINE_AMD64:
        std::puts("x86_64");
        break;
    case IMAGE_FILE_MACHINE_ARM:
        std::puts("arm");
        break;
    case IMAGE_FILE_MACHINE_ARM64:
        std::puts("aarch64");
        break;
    case IMAGE_FILE_MACHINE_IA64:
        std::puts("itanium");
        break;
    default:
        std::ignore = std::fputs("error determining architecture", stderr);
        return -1;
    }
}
