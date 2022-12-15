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
# cmake/get_arch.cmake --
#   CMake function that takes a rather educated guess at the ISA the current
#   build is for.
#   WARNING: This does not take cross-compilation into account, just probes the
#   current system for this information.
#   On Windows this also requires the winarch util to have been compiled, or
#   at least added as a target.

function(get_arch _Arch)
    if (WIN32)
        execute_process(COMMAND winarch
                        OUTPUT_VARIABLE winarch_guess)

        set(${_Arch} "${winarch_guess}" PARENT_SCOPE)
    else ()
        execute_process(COMMAND uname -m
                        OUTPUT_VARIABLE uname_m)

        if (uname_m MATCHES [[x86_64]])
            set(${_Arch} "x86_64" PARENT_SCOPE)
        elseif (uname_m MATCHES [[arm64|aarch64]])
            set(${_Arch} "aarch64" PARENT_SCOPE)
        elseif (uname_m MATCHES [[arm]])
            set(${_Arch} "arm" PARENT_SCOPE)
        else ()
            set(${_Arch} "x86" PARENT_SCOPE)
        endif ()
    endif ()
endfunction()

