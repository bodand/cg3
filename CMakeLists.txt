# cg3 project
#
# Copyright (c) 2022, András Bodor <bodand@proton.me>
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
#   The main CMake script for building and managing the libhaven library.

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
include(CheckIPOSupported)
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
find_package(InfoCLI CONFIG REQUIRED)

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
configure_file(src/chk3/checks.in.hxx chk3/checks.hxx
        @ONLY)
configure_file(src/chk3/check_includes.in.hxx chk3/check_includes.hxx
        @ONLY)

## perform try_compiles
try_compile(HAS_FS_PATH_HASH
        "${CMAKE_CURRENT_BINARY_DIR}/try/fs_path_hash"
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/try/fs_path_hash.cxx"
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

# cg3-common
add_library(cg3-common STATIC
        src/cg3-common/path_finder.cxx
        src/cg3-common/path_finder.hxx
        src/cg3-common/filter.cxx
        src/cg3-common/filter.hxx
        src/cg3-common/boost-json.hxx
        src/cg3-common/execute_process.hxx
        src/cg3-common/execute_process.cxx
        src/cg3-common/find_executable.hxx
        src/cg3-common/hash_storages.hxx
        )
if (WIN32)
    target_sources(cg3-common PRIVATE
            src/cg3-common/find_executable.win32.cxx)
else ()
    target_sources(cg3-common PRIVATE
            src/cg3-common/find_executable.posix.cxx)
endif ()
target_compile_definitions(cg3-common PUBLIC
        "CG3_VERSION_STRING=\"v${PROJECT_VERSION}\""
        "$<$<NOT:$<BOOL:${HAS_FS_PATH_HASH}>>:CG3_STUB_FS_PATH_HASH>")
target_compile_features(cg3-common PUBLIC cxx_std_17)
target_include_directories(cg3-common PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_compile_options(cg3-common PUBLIC $<$<PLATFORM_ID:Windows>:/EHsc>)
target_link_libraries(cg3-common PUBLIC Boost::json reproc++ cg3i-warnings)

# chk3 static lib
add_library(chk3 STATIC
        src/chk3/loader.hxx src/chk3/loader.cxx
        src/chk3/check.hxx src/chk3/check.cxx
        src/chk3/diagnostics_collection.hxx src/chk3/diagnostics_collection.cxx)
target_compile_features(chk3 PUBLIC cxx_std_17)
target_link_libraries(chk3 PUBLIC
        ${llvm_libs}
        clangAST
        clangASTMatchers
        clangBasic
        clangFrontend
        clangSerialization
        clangTooling
        magic_enum::magic_enum
        cg3i-warnings
        cg3-common
        PRIVATE
        cg3i-coverage) # do not leak coverage into consumers
target_compile_definitions(chk3 PUBLIC
        ${LLVM_DEFINITIONS})
target_include_directories(chk3 PUBLIC
        $<BUILD_INTERFACE:${LLVM_INCLUDE_DIRS}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
target_compile_options(chk3 PUBLIC $<$<PLATFORM_ID:Windows>:/EHsc>)

# cg3-check
add_executable(cg3-check
        src/cg3-check/main.cxx
        src/cg3-check/runtime_loader.hxx src/cg3-check/runtime_loader.cxx
        )
target_compile_features(cg3-check PRIVATE cxx_std_17)
target_link_libraries(cg3-check PRIVATE
        chk3
        ${CG3_CHECK_LIBS}
        cg3i-warnings)
target_include_directories(cg3-check PRIVATE
        $<BUILD_INTERFACE:${LLVM_INCLUDE_DIRS}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

# cg3-db
add_executable(cg3-db
        src/cg3-db/main.cxx
        src/cg3-db/path_insert_iterator.hxx
        src/cg3-db/path_insert_iterator.cxx
        src/cg3-db/path_transformer.hxx
        src/cg3-db/path_transformer.cxx
        src/cg3-db/fixup_compiler.hxx
        src/cg3-db/compiler_info.hxx
        src/cg3-db/compiler_info.cxx)
target_compile_features(cg3-db PRIVATE cxx_std_17)
target_include_directories(cg3-db PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(cg3-db PRIVATE info::cli Boost::json cg3-common cg3i-warnings)
target_compile_options(cg3-db PUBLIC $<$<PLATFORM_ID:Windows>:/EHsc>)

# cg3
add_executable(cg3
        src/cg3/main.cxx
        src/cg3/db_cmd.hxx
        src/cg3/db_cmd.cxx
        src/cg3/check_cmd.hxx
        src/cg3/check_cmd.cxx)
add_dependencies(cg3 cg3-db cg3-check)
target_include_directories(cg3 PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(cg3 PRIVATE info::cli cg3-common cg3i-warnings)
target_compile_options(cg3-db PUBLIC $<$<PLATFORM_ID:Windows>:/EHsc>)

# cg3-test-commons
add_library(cg3i-test-commons OBJECT
        src/util/test-commons/test_consumer.hxx
        src/util/test-commons/capture_stream.hxx
        src/util/test-commons/capture_stream.cxx
        src/util/test-commons/test_ast_loader.hxx)
target_include_directories(cg3i-test-commons PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/util>)
target_compile_features(cg3i-test-commons PRIVATE
        cxx_std_20)
target_link_libraries(cg3i-test-commons PUBLIC
        Catch2::Catch2WithMain
        ${llvm_libs}
        clangAST
        clangASTMatchers
        clangBasic
        clangFrontend
        clangSerialization
        clangTooling
        magic_enum::magic_enum
        cg3i-warnings)

# Enable LTO on Release
if (CMAKE_BUILD_TYPE MATCHES [[Rel]])
    check_ipo_supported(RESULT HAVE_LTO)
    if (HAVE_LTO)
        set_target_properties(cg3 cg3-check cg3-db PROPERTIES
                INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif ()
    if (MSVC)
        set_target_properties(cg3 cg3-check cg3-db PROPERTIES
                COMPILE_OPTIONS "/GL"
                LINK_FLAGS "/LTCG:incremental")
    endif ()
endif ()

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
if (WIN32)
    add_executable(winarch src/util/winarch/main.cxx)
    target_compile_options(winarch PRIVATE /EHsc)
    target_compile_features(winarch PRIVATE cxx_std_17)
endif ()
get_arch(PACKAGE_ARCH)
if (NOT PACKAGE_ARCH)
    message(SEND_ERROR "Could not guess meaningfully about package ISA")
endif ()

set(CPACK_PACKAGE_NAME "cg3-checker")
set(CPACK_PACKAGE_FILE_NAME "cg3-checker-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${PACKAGE_ARCH}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "cg3-checker-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${PACKAGE_ARCH}")
set(CPACK_THREADS 0)
set(CPACK_PACKAGE_VENDOR "András Bodor <bodand@proton.me>")
set(CPACK_PACKAGE_CONTACT "András Bodor <bodand@proton.me>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Prog1 C homework static checker")
set(CPACK_PACKAGE_DESCRIPTION "The cg3 project is a prog1 homework static checker built on clang's AST.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.adoc")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.adoc")
set(CPACK_SOURCE_IGNORE_FILES
        "_build.*"
        "\\\\\\\\.idea.*"
        "\\\\\\\\.git/modules/.*"
        "vcpkg/.*"
        )
set(CPACK_STRIP_FILES TRUE)
if (WIN32)
    if (NOT ASCIIDOCTOR_EXE OR NOT PANDOC_EXE)
        message(SEND_ERROR "Cannot build Windows package without asciidoctor and pandoc")
    endif ()
    set(CPACK_GENERATOR WIX)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.docbook"
            VERBATIM COMMAND "${ASCIIDOCTOR_EXE}" -b docbook
            "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.adoc"
            -o "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.docbook"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.adoc")
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.rtf"
            VERBATIM COMMAND "${PANDOC_EXE}"
            -f docbook
            -s "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.docbook"
            -o "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.rtf"
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.docbook")
    add_custom_target(LicenseRTF ALL
            DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.rtf")

    set(cg3_build_triplet "${CG3_SYS_ARCH}-${CG3_SYS_NAME}")
    file(GLOB DEPENDENCY_LIBS
            LIST_DIRECTORIES FALSE
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/${cg3_build_triplet}/bin/*.dll")
    list(FILTER DEPENDENCY_LIBS EXCLUDE REGEX [[(libclang|LLVM-C|Remarks|LTO)\.dll]])
    install(FILES ${DEPENDENCY_LIBS}
            DESTINATION "${CMAKE_INSTALL_BINDIR}")

    set(CPACK_WIX_LICENSE_RTF "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.rtf")
    set(CPACK_WIX_UPGRADE_GUID "BD98979E-CD32-4FDC-8FF2-7A206255E370")
endif ()

# Debian package
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/copyright"
        DESTINATION "${CMAKE_INSTALL_DOCDIR}")
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# RPM package
set(CPACK_RPM_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}.rpm")
set(CPACK_RPM_PACKAGE_LICENSE "BSD")
set(CPACK_RPM_PACKAGE_AUTOREQPROV ON)
set(CPACK_RPM_COMPRESSION_TYPE "xz")

include(CPack)
