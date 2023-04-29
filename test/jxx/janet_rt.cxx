/* cg3 project
 *
 * Copyright (c) 2023 András Bodor <bodand@pm.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * - Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Originally created: 2023-04-28.
 *
 * test/jxx/janet_rt --
 *   
 */

#include <catch2/catch_test_macros.hpp>
#include <jxx/janet_rt.hxx>
#include <jxx/values/integer.hxx>

TEST_CASE("janet_rt can be created",
          "[jxx][janet_rt]") {
    const jxx::janet_rt jrt;
}

TEST_CASE("janet_rt can execute code",
          "[jxx][janet_rt]") {
    jxx::janet_rt jrt;

    auto res = jrt.exec("(+ 1 2 3)");

    CHECK(res.success());
}

TEST_CASE("janet_rt can execute code and returns correct type",
          "[jxx][janet_rt]") {
    jxx::janet_rt jrt;

    auto res = jrt.exec("(+ 1 2 3)");
    REQUIRE(res.success());

    CHECK(res->is<jxx::integer<32>>());
}

TEST_CASE("janet_rt can execute code and returns correct value",
          "[jxx][janet_rt]") {
    jxx::janet_rt jrt;

    auto res = jrt.exec("(+ 1 2 3)");
    REQUIRE(res.success());
    REQUIRE(res->is<jxx::integer<32>>());

    CHECK(jxx::integer<32>(res.value()) == 6);
}
