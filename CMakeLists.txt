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
        VERSION 0.1.1
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
include(CTest)
set(CMAKE_INSTALL_RPATH "$ORIGIN:/lib64:/usr/lib64:/usr/local/lib64:/lib:/usr/lib:/usr/local/lib")

# our modules
include(cmake/add_ast.cmake)
include(cmake/add_check.cmake)
include(cmake/add_manpage.cmake)
include(cmake/add_htmldoc.cmake)

find_program(ASCIIDOCTOR_EXE asciidoctor
             DOC "The asciidoctor executable used to build documentation")
find_program(PANDOC_EXE pandoc
             DOC "The pandoc executable used to convert documentation formats")

# build options
set(CG3_BUILD_CHECKS "bugmalloc;globus;fio;fleak;arityck;t;hunction;chonktion" CACHE STRING
    "The ;-separated list of checks to build")

# Dependencies
find_package(LLVM CONFIG REQUIRED)
find_package(Clang CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(lyra CONFIG REQUIRED)
find_package(Boost REQUIRED json)
find_package(ut CONFIG REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CLANG_CMAKE_DIR}")
include(HandleLLVMOptions)

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

# chk3 static lib
add_library(chk3 STATIC
            src/chk3/loader.hxx src/chk3/loader.cxx
            src/chk3/check.hxx src/chk3/check.cxx)
target_compile_features(chk3 PUBLIC cxx_std_17)
target_link_libraries(chk3 PUBLIC
                      ${llvm_libs}
                      clangAST
                      clangASTMatchers
                      clangBasic
                      clangFrontend
                      clangSerialization
                      clangTooling
                      magic_enum::magic_enum)
target_compile_definitions(chk3 PUBLIC
                           ${LLVM_DEFINITIONS})
target_include_directories(chk3 PUBLIC
                           $<BUILD_INTERFACE:${LLVM_INCLUDE_DIRS}>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
if (MSVC)
    target_compile_options(chk3 PUBLIC /EHsc)
endif ()

# cg3-check
add_executable(cg3-check
               src/cg3/main.cxx
               src/cg3/runtime_loader.hxx src/cg3/runtime_loader.cxx
               )
target_compile_features(cg3-check PRIVATE cxx_std_17)
target_link_libraries(cg3-check PRIVATE
                      chk3
                      ${CG3_CHECK_LIBS})
target_include_directories(cg3-check PRIVATE
                           $<BUILD_INTERFACE:${LLVM_INCLUDE_DIRS}>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
# cg3-db
add_executable(cg3-db
               src/cg3-db/main.cxx
               src/cg3-db/parse_dir.hxx
               src/cg3-db/path_insert_iterator.hxx
               src/cg3-db/path_insert_iterator.cxx
               src/cg3-db/path_transformer.hxx
               src/cg3-db/path_transformer.cxx
               src/cg3-db/fixup_compiler.hxx
               $<$<PLATFORM_ID:Windows>:src/cg3-db/fixup_compiler.win32.cxx>
               $<$<NOT:$<PLATFORM_ID:Windows>>:src/cg3-db/fixup_compiler.posix.cxx>
               src/cg3-db/compiler_info.cxx src/cg3-db/compiler_info.hxx)
target_compile_features(cg3-db PRIVATE cxx_std_17)
target_include_directories(cg3-db PRIVATE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(cg3-db PRIVATE bfg::lyra Boost::json)
target_compile_options(cg3-db PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/EHsc>)

if (CMAKE_BUILD_TYPE MATCHES [[Rel]])
    check_ipo_supported(RESULT HAVE_LTO)
    if (HAVE_LTO)
        set_target_properties(cg3-check cg3-db PROPERTIES
                              INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif ()
    if (MSVC)
        set_target_properties(cg3-check cg3-db PROPERTIES
                              COMPILE_OPTIONS "/GL"
                              LINK_FLAGS "/LTCG:incremental")
    endif ()
endif ()

# docs
add_subdirectory(docs)

# Installation
install(TARGETS cg3-check cg3-db
        DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.adoc"
        DESTINATION "${CMAKE_INSTALL_DOCDIR}")

# cpack
set(CPACK_PACKAGE_NAME "cg3-checker")
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

    file(GLOB DEPENDENCY_LIBS
         LIST_DIRECTORIES FALSE
         CONFIGURE_DEPENDS
         "${CMAKE_CURRENT_BINARY_DIR}/vcpkg-master/installed/${VCPKG_TRIPLET}/bin/*.dll")
    install(FILES ${DEPENDENCY_LIBS}
            DESTINATION "${CMAKE_INSTALL_BINDIR}")

    set(CPACK_WIX_LICENSE_RTF "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.rtf")
    set(CPACK_WIX_UPGRADE_GUID "BD98979E-CD32-4FDC-8FF2-7A206255E370")
endif ()
if (LINUX)
    # Debian package
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/copyright"
            DESTINATION "${CMAKE_INSTALL_DOCDIR}")
    set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

    # RPM package
    set(CPACK_RPM_PACKAGE_LICENSE "BSD")
    set(CPACK_RPM_PACKAGE_AUTOREQPROV ON)
    set(CPACK_RPM_COMPRESSION_TYPE "xz")
endif ()
include(CPack)
