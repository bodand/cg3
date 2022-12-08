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
# docs/man/CMakeLists.txt --
#   The CMake script for building and installing the manual pages of the
#   documentation of cg3.
#   Manpages are also compiled to html and installed to the documentation
#   directory, for those not used to the terminal, and Windows users.

add_manpage(cg3.1.adoc)
add_manpage(cg3-db.1.adoc)
add_htmldoc(cg3.html cg3.1.adoc)
add_htmldoc(cg3-db.html cg3-db.1.adoc)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cg3.1"
        "${CMAKE_CURRENT_BINARY_DIR}/cg3-db.1"
        DESTINATION
        "${CMAKE_INSTALL_MANDIR}/man1")
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cg3.html"
        "${CMAKE_CURRENT_BINARY_DIR}/cg3-db.html"
        DESTINATION
        "${CMAKE_INSTALL_DOCDIR}/html")
