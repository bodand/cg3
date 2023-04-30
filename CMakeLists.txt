# cg3 project
#
# Copyright (c) 2022, 2023, András Bodor <bodand@proton.me>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# - Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# - Neither the name of the copyright holder nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# CMakeLists.txt --
#   The main CMake script for building and managing the cg3 project.

cmake_minimum_required(VERSION 3.22)
enable_language(C)
enable_language(CXX)

# CMake policies ###############################################################
# Set CMake policies to the expected behavior.

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
    cmake_policy(SET CMP0135 NEW)
endif ()
cmake_policy(SET CMP0127 NEW)
cmake_policy(SET CMP0125 NEW)
cmake_policy(SET CMP0118 NEW)
cmake_policy(SET CMP0115 NEW)
cmake_policy(SET CMP0111 NEW)
cmake_policy(SET CMP0110 NEW)
cmake_policy(SET CMP0096 NEW)
cmake_policy(SET CMP0092 NEW)
cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0048 NEW)

# project #########################################################################
project(cg3
        VERSION 0.2.2
        HOMEPAGE_URL "https://github.com/bodand/cg3"
        DESCRIPTION "Prog1 homework static checker"
        LANGUAGES CXX)

# generate versioninfo.txt file for build
# do not remove, needed for CI/CD
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/versioninfo.txt" "${PROJECT_VERSION}")

# Include modules
include(GNUInstallDirs)
include(CheckCXXSymbolExists)
include(CMakeDependentOption)
include(InstallRequiredSystemLibraries)
include(FetchContent)
include(CTest)

# our modules
include(cmake/add_ast.cmake)
include(cmake/add_check.cmake)
include(cmake/add_htmldoc.cmake)
include(cmake/add_manpage.cmake)
include(cmake/add_covered_test.cmake)
include(cmake/generate_warnings.cmake)
include(cmake/generate_lto.cmake)
include(cmake/get_arch.cmake)

find_program(ASCIIDOCTOR_EXE asciidoctor
             DOC "The asciidoctor executable used to build documentation")
find_program(PANDOC_EXE pandoc
             DOC "The pandoc executable used to convert documentation formats")

# build options
set(CG3_BUILD_CHECKS "bugmalloc;globus;fio;fleak;arityck;t;hunction;chonktion" CACHE STRING
    "The ;-separated list of checks to build")

# coverage data
add_library(cg3i-coverage INTERFACE)
if (NOT DEFINED ENV{CG_DEPLOYMENT})
    target_compile_options(cg3i-coverage INTERFACE
                           $<$<CXX_COMPILER_ID:Clang>:-fprofile-instr-generate>
                           $<$<CXX_COMPILER_ID:Clang>:-fcoverage-mapping>)
    if ("x${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}" STREQUAL "xMSVC")
        cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH vs_llvmx64_bindir)
        cmake_path(GET vs_llvmx64_bindir PARENT_PATH vs_llvmx64_dir)
        set(clang_profile_lib "${vs_llvmx64_dir}/lib/clang/15.0.1/lib/windows/clang_rt.profile-x86_64.lib")
        target_link_libraries(cg3i-coverage INTERFACE "${clang_profile_lib}")
    else ()
        target_link_options(cg3i-coverage INTERFACE
                            $<$<CXX_COMPILER_ID:Clang>:-fprofile-instr-generate>
                            $<$<CXX_COMPILER_ID:Clang>:-fcoverage-mapping>)
    endif ()
endif ()

# warnings
add_library(cg3i-warnings INTERFACE)
generate_warnings(cg3i-warnings)

# LTO
add_library(cg3i-lto INTERFACE)
generate_lto(cg3i-lto)

# Dependencies
if ("x${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}" STREQUAL "xMSVC")
    message(STATUS "Using clang-cl: ${CMAKE_CXX_COMPILER}")
    message(STATUS "Injecting /EHsc into clang-cl command-line")
    add_compile_options(/EHsc)
endif ()
find_package(LLVM CONFIG REQUIRED)
find_package(Clang CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(Boost REQUIRED json)
find_package(Catch2 CONFIG REQUIRED)
find_package(reproc++ CONFIG REQUIRED)
find_package(InfoUtils CONFIG REQUIRED)
find_package(InfoCLI CONFIG REQUIRED)
find_package(mimalloc CONFIG REQUIRED)

add_subdirectory(ext/janet)
add_subdirectory(ext/janetxx)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CLANG_CMAKE_DIR}")
include(HandleLLVMOptions)
include(Catch)

llvm_map_components_to_libnames(llvm_libs Support)

# checks
foreach (chk IN LISTS CG3_BUILD_CHECKS)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/checks/${chk}/CMakeLists.txt")
        add_subdirectory("checks/${chk}")
    else ()
        message(WARNING
                "Unknown check specified to CG3_BUILD_CHECKS: ${chk}")
        list(REMOVE_ITEM CG3_BUILD_CHECKS "${chk}")
    endif ()
endforeach ()

## generate checks enum
string(REPLACE ";" ",
" CG3_CHECK_TYPES_ENUM "${CG3_BUILD_CHECKS}")
string(REPLACE ";" ",check_types::" CG3_CHECK_TYPES_NAMESPACED "${CG3_BUILD_CHECKS}")
set(CG3_CHECK_TYPES_NAMESPACED "check_types::${CG3_CHECK_TYPES_NAMESPACED}")

## generate checks descs
foreach (chk IN LISTS CG3_BUILD_CHECKS)
    string(TOUPPER "${chk}" CHK_NAME)

    set(CG3_CHECK_TYPES_DESCRIPTION_IMPLS
        "${CG3_CHECK_TYPES_DESCRIPTION_IMPLS}
template<>
struct check_description_impl<check_types::${chk}> {
    constexpr const static auto value = \"${CG3_CHECK_${CHK_NAME}_DESC}\";
};")
    set(CG3_CHECK_TYPES_NAME_IMPLS
        "${CG3_CHECK_TYPES_NAME_IMPLS}
template<>
struct check_name_impl<check_types::${chk}> {
    constexpr const static auto value = \"${chk}\";
};")
    set(CG3_CHECK_INCLUDES
        "${CG3_CHECK_INCLUDES}
#include <${CG3_LOADER_${CHK_NAME}}>")

    list(APPEND CG3_CHECK_LIBS "cg3-check-${chk}")
endforeach ()

## configure checks
## perform try_compiles
try_compile(HAS_FS_PATH_HASH
            "${CMAKE_CURRENT_BINARY_DIR}/try/fs_path_hash"
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/try/fs_path_hash.cxx"
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO)

# targets
add_subdirectory(src/cg3-common)
add_subdirectory(src/cg3-api)
add_subdirectory(src/chk3)
add_subdirectory(src/cg3-check)
add_subdirectory(src/cg3-db)
add_subdirectory(src/cg3-shell)
add_subdirectory(src/cg3)

# docs
add_subdirectory(docs)
# test
add_subdirectory(test)

# clang-tidy
if (CMAKE_GENERATOR MATCHES [[Make|Ninja]]
    AND (NOT DEFINED ENV{CI} OR DEFINED ENV{CG3_CI_DO_CLANG_TIDY}))
    # CMAKE_EXPORT_COMPILE_COMMANDS is only implemented on Make and Ninja...
    cmake_path(RELATIVE_PATH CMAKE_CURRENT_BINARY_DIR OUTPUT_VARIABLE rel_bindir)
    foreach (tgt cg3 cg3-check cg3-db
             cg3-common chk3 ${CG3_CHECK_LIBS})
        get_target_property(tgt_dir "${tgt}" SOURCE_DIR)
        get_target_property(tgt_srces "${tgt}" SOURCES)
        get_target_property(tgt_isrces "${tgt}" INTERFACE_SOURCES)
        foreach (src IN LISTS tgt_srces tgt_isrces)
            if (NOT src)
                continue()
            endif ()
            cmake_path(ABSOLUTE_PATH src BASE_DIRECTORY "${tgt_dir}")
            cmake_path(RELATIVE_PATH src BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

            add_test(NAME "clang-tidy whole project/${src}"
                     COMMAND "clang-tidy" -p "${rel_bindir}" ${src}
                     WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                     COMMAND_EXPAND_LISTS)
        endforeach ()
    endforeach ()
endif ()

# Installation
install(TARGETS cg3 cg3-check cg3-db
        DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.adoc"
        DESTINATION "${CMAKE_INSTALL_DOCDIR}")

# cpack
include(cmake/cpack_config.cmake)