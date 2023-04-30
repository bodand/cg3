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
# Originally created: 2023-04-28.
#
# ext/janetxx/CMakeLists.txt --
#   A header-only wrapper library around janet's C api in C++. Provides various
#   QoL improvements over any C API, like explicit typing, and various RAII
#   classes.

set(jxx_is_debug no)
if (CMAKE_BUILD_TYPE MATCHES [[Debug]])
    set(jxx_is_debug yes)
endif ()

option(JXX_CHECK_THREAD
       "Performs checks that the runtime is used on the same thread as constructed [Debug]"
       ${jxx_is_debug})
option(JXX_CHECK_NULL_PARANOID
       "Perform null checks on basically every pointer parameter [Debug]"
       ${jxx_is_debug})
option(JXX_CHECK_NULL_VM
       "Perform null checks on pointers not directly received from the VM [Debug]"
       ${jxx_is_debug})
option(JXX_CHECK_DANGLING_RT_PROXY
       "Perform checks whether there are proxy objects belonging to the runtime when that is destructed [On]"
       On)

## null check level
set(jxx_null_check_level 0)
if (JXX_CHECK_NULL_PARANOID)
    math(EXPR jxx_null_check_level "${jxx_null_check_level} + 1")
endif ()
if (JXX_CHECK_NULL_VM)
    math(EXPR jxx_null_check_level "${jxx_null_check_level} + 1")
endif ()

add_library(cg3-janetxx INTERFACE
            src/jxx/janet_rt.hxx
            src/jxx/func.hxx
            src/jxx/impl/definer_proxy.hxx
            src/jxx/impl/dissector.hxx
            src/jxx/values/types.hxx
            src/jxx/values/function.hxx
            src/jxx/values/integer.hxx
            src/jxx/values/boolean.hxx
            src/jxx/values/string.hxx
            )
target_include_directories(cg3-janetxx INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(cg3-janetxx INTERFACE cg3-janet cg3i-warnings info::utils)
target_compile_features(cg3-janetxx INTERFACE cxx_std_20)

target_compile_definitions(cg3-janetxx INTERFACE
                           $<$<BOOL:${JXX_CHECK_THREAD}>:JXX_CHECK_THREAD>
                           $<$<BOOL:${JXX_CHECK_DANGLING_RT_PROXY}>:JXX_CHECK_DANGLING_RT_PROXY>
                           JXX_NULL_CHECK_LVL=${jxx_null_check_level})
target_compile_options(cg3-janetxx INTERFACE
                       $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:Windows>>>:/EHsc>)