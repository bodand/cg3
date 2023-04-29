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
# src/cg3-shell/CMakeLists.txt --
#   The cg3-shell executable's implementation which provides a Janet shell
#   after loading cg3-api into it.

project(cg3-shell C CXX)

set(mi_stats Off)
if (CMAKE_BUILD_TYPE MATCHES [[Debug]])
    set(mi_stats On)
endif ()

option(CG3_MI_CHECK_HEAP "Perform heap checks using the mimalloc statistics. [Debug]" ${mi_stats})

add_executable(cg3-shell
               src/cg3-shell/shell.c)
target_link_libraries(cg3-shell PRIVATE
                      cg3-janet
                      cg3-janetxx
                      cg3i-warnings
                      cg3i-lto
                      )
target_compile_definitions(cg3-shell PRIVATE
                           $<$<BOOL:${CG3_MI_CHECK_HEAP}>:CG3_MI_CHECK_HEAP>)
target_include_directories(cg3-shell PRIVATE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_compile_options(cg3-shell PUBLIC $<$<PLATFORM_ID:Windows>:/EHsc>)
