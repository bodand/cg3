/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 26.
 *
 * src/chk3/diagnostics_collection --
 */

#include <cassert>
#include <numeric>
#include <type_traits>

#include <chk3/diagnostics_collection.hxx>
#include <magic_enum.hpp>

cg3::diagnostic_chain
cg3::diagnostics_collection::new_chain(cg3::check_types type) {
    auto type_idx = static_cast<
           std::make_unsigned_t<magic_enum::underlying_type_t<cg3::check_types>>>(type);

    auto&& type_diags = _diagnostics[type_idx];
    type_diags.emplace_back();

    auto latest_chain_idx = type_diags.size() - 1;
    return diagnostic_chain{type, latest_chain_idx};
}

cg3::diagnostic_chain
cg3::diagnostics_collection::chain_diagnostic(cg3::diagnostic_chain chain, const cg3::diagnostic& diag) {
    auto [type, chain_idx] = chain;
    auto type_idx = static_cast<
           std::make_unsigned_t<magic_enum::underlying_type_t<cg3::check_types>>>(type);

    assert(type_idx < _diagnostics.size());
    assert(chain_idx < _diagnostics[type_idx].size());

    _diagnostics[type_idx][chain_idx].push_back(diag);

    return chain;
}

cg3::diagnostic_chain
cg3::diagnostics_collection::chain_diagnostic(cg3::check_types type, const cg3::diagnostic& diag) {
    auto new_chain = this->new_chain(type);
    return chain_diagnostic(new_chain, diag);
}

std::size_t
cg3::diagnostics_collection::chain_count() const {
    return std::accumulate(_diagnostics.begin(),
                           _diagnostics.end(),
                           std::size_t{0U},
                           [](auto acc, const auto& vec) { return acc + std::size(vec); });
}
