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
# src/cg3-common/CMakeLists.txt --
#   Common functionality used by multiple parts of the code. Bascially a set
#   of utility functions and types.

project(cg3-common)

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
    target_sources(cg3-common PRIVATE src/cg3-common/find_executable.win32.cxx)
else ()
    target_sources(cg3-common PRIVATE src/cg3-common/find_executable.posix.cxx)
endif ()
target_compile_definitions(cg3-common PUBLIC
                           "CG3_VERSION_STRING=\"v${PROJECT_VERSION}\""
                           "$<$<NOT:$<BOOL:${HAS_FS_PATH_HASH}>>:CG3_STUB_FS_PATH_HASH>")
target_compile_features(cg3-common PUBLIC cxx_std_17)
target_include_directories(cg3-common PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_compile_options(cg3-common PUBLIC $<$<PLATFORM_ID:Windows>:/EHsc>)
target_link_libraries(cg3-common PUBLIC Boost::json reproc++ cg3i-warnings cg3i-lto)
