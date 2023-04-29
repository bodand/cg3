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
 * Originally created: 2023-04-29.
 *
 * src/cg3-api/matcher_funcs --
 *   
 */

#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <janet.h>

using namespace clang::ast_matchers;

extern "C" Janet
cg3_mk_matcher_hasGlobalStorageMatcher(std::int32_t argc, Janet*) {
    janet_fixarity(argc, 0);

    auto* res = janet_struct_begin(2);
    janet_struct_put(res,
                     janet_ckeywordv("type"),
                     janet_ckeywordv("VarDecl"));
    janet_struct_put(res,
                     janet_ckeywordv("value"),
                     janet_wrap_abstract(new internal::matcher_hasGlobalStorageMatcher()));

    return janet_wrap_struct(res);
}

extern "C" Janet
cg3_mk_matcher_varDecl(std::int32_t argc, Janet* argv) {
    janet_arity(argc, 0, -1);

    std::vector<internal::Matcher<clang::VarDecl>> params;
    for (int i = 0; i < argc; ++i) {
        if (!janet_checktypes(argv[i], JANET_TFLAG_STRUCT)) {
            janet_panicf("invalid parameter passed in %d-th argument of varDecl", i);
        }

        auto* data = janet_unwrap_struct(argv[i]);
        auto jtype = janet_struct_get(data, janet_ckeywordv("type"));
        if (!janet_keyeq(jtype, "VarDecl")) continue;

        auto jmatcher = janet_struct_get(data, janet_ckeywordv("value"));
        if (!janet_checktypes(jmatcher, JANET_TFLAG_ABSTRACT)) continue;
        auto* matcher = reinterpret_cast<internal::MatcherInterface<clang::VarDecl>*>(janet_unwrap_abstract(jmatcher));
        params.emplace_back(matcher);
    }

    internal::Matcher<clang::Decl> m_decl(varDecl(params));
}
