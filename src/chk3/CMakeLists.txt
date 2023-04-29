# cg3 project
#
# Copyright (c) 2023 András Bodor <bodand@pm.me>
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
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Originally created: 2023-04-29.
#
# src/chk3/CMakeLists.txt --
#   The library implementing the functionality required by the different
#   legacy checks.

project(ch3-chk3)

configure_file(src/chk3/checks.in.hxx chk3/checks.hxx
               @ONLY)
configure_file(src/chk3/check_includes.in.hxx chk3/check_includes.hxx
               @ONLY)

add_library(chk3 STATIC
            src/chk3/loader.hxx
            src/chk3/loader.cxx
            src/chk3/check.hxx
            src/chk3/check.cxx
            src/chk3/diagnostics_collection.hxx
            src/chk3/diagnostics_collection.cxx
            src/chk3/collecting_consumer.cxx
            src/chk3/collecting_consumer.hxx
            src/chk3/diagnostics_exporter.cxx
            src/chk3/diagnostics_exporter.hxx
            src/chk3/text_exporter.cxx
            src/chk3/text_exporter.hxx)
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
                      cg3i-lto
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

