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
# checks/arityck/test/CMakeLists.txt --
#    Tests for the arityck check

add_executable(cg3-check-arityck-test
        check_spec.cxx
        ast_tests.cxx)

add_ast("data/empty.c" "data/empty.c.ast")
add_ast("data/empty.cxx" "data/empty.cxx.ast")
add_ast("data/ok.c" "data/ok.c.ast")
add_ast("data/ok.cxx" "data/ok.cxx.ast")
add_ast("data/bad.c" "data/bad.c.ast")
add_ast("data/bad.cxx" "data/bad.cxx.ast")
add_custom_target(cg3-check-arityck-test-data
        DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/data/empty.c.ast"
        "${CMAKE_CURRENT_BINARY_DIR}/data/ok.c.ast"
        "${CMAKE_CURRENT_BINARY_DIR}/data/bad.c.ast"
        "${CMAKE_CURRENT_BINARY_DIR}/data/empty.cxx.ast"
        "${CMAKE_CURRENT_BINARY_DIR}/data/ok.cxx.ast"
        "${CMAKE_CURRENT_BINARY_DIR}/data/bad.cxx.ast"
        )

add_dependencies(cg3-check-arityck-test
        cg3-check-arityck-test-data)

target_link_libraries(cg3-check-arityck-test PRIVATE
        chk3
        cg3-check-arityck
        cg3i-test-commons)
target_compile_features(cg3-check-arityck-test PRIVATE
        cxx_std_20)

catch_discover_tests(cg3-check-arityck-test
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
