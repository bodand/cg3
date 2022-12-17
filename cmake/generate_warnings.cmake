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
# cmake/generate_warnings.cmake --
#   Generates a set of warnings available on the currently used compiler.
#   Sets them on the given interface target; this is to be used while compiling
#   every other target to all have the full set of available warnings.

include(CheckCXXCompilerFlag)

function(CheckWarningFlag OptionName CacheName)
    if (OptionName MATCHES [[^/]]) # MSVC-style args are passed as-is
        set(WarningPrefix "")
    else ()
        set(WarningPrefix "-W")
    endif ()
    check_cxx_compiler_flag("${WarningPrefix}${OptionName}" "HasWarning_${CacheName}")
    set("HAS_WARNING_${CacheName}" ${HasWarning_${CacheName}} PARENT_SCOPE)
endfunction()

function(generate_warnings _Interface)
    set(gw_known_warnings
        extra pedantic sign-compare uninitialized unused cast-qual cast-align
        abstract-vbase-init array-bounds-pointer-arithmetic assign-enum conditional-uninitialized
        consumed deprecated-implementations documentation-deprecated-sync non-virtual-dtor
        header-hygiene infinite-recursion keyword-macro loop-analysis
        move newline-eof over-aligned redundant-parens reorder reserved-id-macro sign-conversion
        signed-enum-bitfield sometimes-uninitialized tautological-overlap-compare thread-safety
        undefined-internal-type undefined-reinterpret-cast unneeded-internal-declaration
        unreachable-code-aggressive unreachable-code-loop-increment unused-const-variable
        unused-exception-parameter unused-parameter unused-template unused-variable nullability-completeness
        no-unknown-pragmas no-unused-macros no-nullability-extension
        suggest-attribute=pure suggest-attribute=const suggest-attribute=cold suggest-final-types
        suggest-final-methods duplicated-branches trampolines placement-new=2 redundant-decls logical-op
        /w14062 /w14165 /w14191 /w14242 /we4263 /w14265 /w14287 /w14296 /we4350 /we4355
        /w14355 /w14471 /we4545 /w14546 /w14547 /w14548 /w14549 /w14557 /we4596 /w14605
        /w14668 /w14768 /w14822 /we4837 /we4928 /we4946 /we4986 /w15032 /w15039 /wd4010
        3
        /diagnostics:caret
        )
    # cannot just check if -Wall, because MSVC also has /Wall, but also accepts -Wall
    # this wouldn't be a problem, but /Wall means literally everything... don't.
    set(gw_found_warnings $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>)

    foreach (warn IN LISTS gw_known_warnings)
        string(MAKE_C_IDENTIFIER "${warn}" CacheName)
        CheckWarningFlag("${warn}" ${CacheName})
        if (HAS_WARNING_${CacheName})
            list(APPEND gw_found_warnings "-W${warn}")
        endif ()
    endforeach ()

    target_compile_options("${_Interface}" INTERFACE ${gw_found_warnings})
endfunction()
