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
# cmake/add_check.cmake --
#   Macro for easing creation of checks

macro(add_check chk version desc loader)
    project("chk-${chk}"
            VERSION ${version}
            LANGUAGES CXX)

    string(TOUPPER "${chk}" "CHK_NAME")

    set("CG3_CHECK_${CHK_NAME}_DESC" "${desc}" CACHE INTERNAL
        "Description of check '${chk}'" FORCE)

    add_library("cg3-check-${chk}" STATIC ${loader} ${ARGN})
    target_include_directories("cg3-check-${chk}" PUBLIC
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

    llvm_map_components_to_libnames("${chk}_llvm_libs" Support)
    target_link_libraries("cg3-check-${chk}" PRIVATE
            chk3
            ${${chk}_llvm_libs}
            clangAST
            clangASTMatchers
            clangBasic
            clangFrontend
            clangSerialization
            clangTooling
            cg3i-warnings
            cg3i-coverage)

    set("CG3_LOADER_${CHK_NAME}" "${CMAKE_CURRENT_SOURCE_DIR}/${loader}" CACHE INTERNAL
        "Loader of the check '${chk}'" FORCE)
    add_library("cg3::${chk}" ALIAS "cg3-check-${chk}")
endmacro()
