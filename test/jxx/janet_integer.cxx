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
 * test/jxx/janet_integer --
 *   
 */

#include <catch2/catch_test_macros.hpp>
#include <jxx/janet_rt.hxx>
#include <jxx/values/integer.hxx>

TEST_CASE("integer<32> can be constructed from integer constant") {
    jxx::integer<32> i = 32;
    CHECK(i == 32);
}

TEST_CASE("integer<64> can be constructed from integer constant") {
    jxx::integer<64> i = 64;
    CHECK(i == 64);
}

TEST_CASE("integer<32> can be compared equal to integer<32>") {
    jxx::integer<> a = 1;
    jxx::integer<> b = 1;
    jxx::integer<> c = 2;

    SECTION("== returns true when they are equal") {
        CHECK(a == b);
    }
    SECTION("== returns false when they are not equal") {
        CHECK_FALSE(a == c);
    }
}

TEST_CASE("integer<64> can be compared equal to integer<64>") {
    jxx::integer<64> a = 1;
    jxx::integer<64> b = 1;
    jxx::integer<64> c = 2;

    SECTION("== returns true when they are equal") {
        CHECK(a == b);
    }
    SECTION("== returns false when they are not equal") {
        CHECK_FALSE(a == c);
    }
}

TEST_CASE("integer<32> can be compared equal to int") {
    jxx::integer<> a = 1;
    int b = 1;
    int c = 2;

    SECTION("== returns true when they are equal") {
        CHECK(a == b);
    }
    SECTION("== returns false when they are not equal") {
        CHECK_FALSE(a == c);
    }
}

TEST_CASE("integer<64> can be compared equal to int") {
    jxx::integer<64> a = 1;
    int b = 1;
    int c = 2;

    SECTION("== returns true when they are equal") {
        CHECK(a == b);
    }
    SECTION("== returns false when they are not equal") {
        CHECK_FALSE(a == c);
    }
}

TEST_CASE("integer<32> can be compared unequal to integer<32>") {
    jxx::integer<> a = 1;
    jxx::integer<> b = 1;
    jxx::integer<> c = 2;

    SECTION("!= returns false when they are equal") {
        CHECK_FALSE(a != b);
    }
    SECTION("!= returns true when they are not equal") {
        CHECK(a != c);
    }
}

TEST_CASE("integer<64> can be compared unequal to integer<64>") {
    jxx::integer<64> a = 1;
    jxx::integer<64> b = 1;
    jxx::integer<64> c = 2;

    SECTION("!= returns false when they are equal") {
        CHECK_FALSE(a != b);
    }
    SECTION("!= returns true when they are not equal") {
        CHECK(a != c);
    }
}

TEST_CASE("integer<32> can be compared unequal to int") {
    jxx::integer<> a = 1;
    int b = 1;
    int c = 2;

    SECTION("!= returns false when they are equal") {
        CHECK_FALSE(a != b);
    }
    SECTION("!= returns true when they are not equal") {
        CHECK(a != c);
    }
}

TEST_CASE("integer<64> can be compared unequal to int") {
    jxx::integer<64> a = 1;
    int b = 1;
    int c = 2;

    SECTION("!= returns false when they are equal") {
        CHECK_FALSE(a != b);
    }
    SECTION("!= returns trrue when they are not equal") {
        CHECK(a != c);
    }
}

TEST_CASE("integer<32> can be compared equal to integer<64>") {
    jxx::integer<32> a = 1;
    jxx::integer<64> b = 1;
    jxx::integer<64> c = 2;

    SECTION("== returns true when they are equal") {
        CHECK(a == b);
    }
    SECTION("== returns false when they are not equal") {
        CHECK_FALSE(a == c);
    }
}

TEST_CASE("integer<32> can be compared unequal to integer<64>") {
    jxx::integer<32> a = 1;
    jxx::integer<64> b = 1;
    jxx::integer<64> c = 2;

    SECTION("!= returns false when they are equal") {
        CHECK_FALSE(a != b);
    }
    SECTION("!= returns true when they are not equal") {
        CHECK(a != c);
    }
}

TEST_CASE("integer<64> can be compared equal to integer<32>") {
    jxx::integer<64> a = 1;
    jxx::integer<32> b = 1;
    jxx::integer<32> c = 2;

    SECTION("== returns true when they are equal") {
        CHECK(a == b);
    }
    SECTION("== returns false when they are not equal") {
        CHECK_FALSE(a == c);
    }
}

TEST_CASE("integer<64> can be compared unequal to integer<32>") {
    jxx::integer<64> a = 1;
    jxx::integer<32> b = 1;
    jxx::integer<32> c = 2;

    SECTION("!= returns false when they are equal") {
        CHECK_FALSE(a != b);
    }
    SECTION("!= returns true when they are not equal") {
        CHECK(a != c);
    }
}
