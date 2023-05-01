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

// see end of file
#include <cg3-api/load_vm.hxx>
#include <jxx/func.hxx>

#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <janet.h>

#include "jxx/abstract_type_cast_exception.hxx"

using namespace clang::ast_matchers;

constexpr const auto decl_matcher_name = "Matcher<Decl>";
static JanetAbstractType DeclMatcher;
constexpr const auto vardecl_matcher_name = "Matcher<VarDecl>";
static JanetAbstractType VarDeclMatcher;

namespace {
    Janet
    mk_hasGlobalStorage() {
        void* res_storage = janet_abstract_begin(&VarDeclMatcher,
                                                 sizeof(internal::matcher_hasGlobalStorageMatcher));
        std::construct_at(static_cast<internal::matcher_hasGlobalStorageMatcher*>(res_storage));

        return janet_wrap_abstract(res_storage);
    }

    Janet
    mk_varDecl(jxx::varargs args) {
        std::vector<internal::Matcher<clang::VarDecl>> params;
        params.reserve(args.argc);

        const auto* expected_type = janet_get_abstract_type(janet_csymbolv(vardecl_matcher_name));

        for (std::size_t i = 0; i < args.argc; ++i) {
            auto&& arg = args.argv[i];
            if (!janet_checktypes(arg, JANET_TFLAG_ABSTRACT))
                throw jxx::native_conversion_error(arg, i, JANET_TFLAG_ABSTRACT);

            auto* abs_vardecl = janet_unwrap_abstract(arg);
            auto* abs_type = janet_abstract_type(abs_vardecl);
            if (abs_type != expected_type)
                throw jxx::abstract_type_cast_exception(abs_type->name, expected_type->name);

            params.emplace_back(
                   static_cast<internal::MatcherInterface<clang::VarDecl>*>(abs_vardecl));
        }
        auto* res_storage = janet_abstract_begin(&DeclMatcher,
                                                 sizeof(internal::BindableMatcher<clang::Decl>));
        std::construct_at(
               static_cast<internal::BindableMatcher<clang::Decl>*>(res_storage),
               varDecl(params));

        return janet_wrap_abstract(res_storage);
    }
}

#define PUT_MATCHER_MAKER(matcher) \
    rt[#matcher] = JXX_DELEGATE(&mk_##matcher)

void
cg3::load_vm(jxx::janet_rt& rt) {

    //    struct JanetAbstractType {
    //        const char* name;
    //        int (*gc)(void* data, size_t len);
    //        int (*gcmark)(void* data, size_t len);
    //        int (*get)(void* data, Janet key, Janet* out);
    //        void (*put)(void* data, Janet key, Janet value);
    //        void (*marshal)(void* p, JanetMarshalContext* ctx);
    //        void* (*unmarshal)(JanetMarshalContext* ctx);
    //        void (*tostring)(void* p, JanetBuffer* buffer);
    //        int (*compare)(void* lhs, void* rhs);
    //        int32_t (*hash)(void* p, size_t len);
    //        Janet (*next)(void* p, Janet key);
    //        Janet (*call)(void* p, int32_t argc, Janet* argv);
    //        size_t (*length)(void* p, size_t len);
    //        JanetByteView (*bytes)(void* p, size_t len);
    //    };
    DeclMatcher.name = decl_matcher_name;
    DeclMatcher.gc = +[](void* data, size_t) {
        std::destroy_at(static_cast<internal::Matcher<clang::Decl>*>(data));
        return 0;
    };
    VarDeclMatcher.name = vardecl_matcher_name;
    VarDeclMatcher.gc = +[](void* data, size_t) {
        std::destroy_at(static_cast<internal::Matcher<clang::VarDecl>*>(data));
        return 0;
    };

    janet_register_abstract_type(&DeclMatcher);
    janet_register_abstract_type(&VarDeclMatcher);

    PUT_MATCHER_MAKER(hasGlobalStorage);
    PUT_MATCHER_MAKER(varDecl);
}
