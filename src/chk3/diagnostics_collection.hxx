/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 26.
 *
 * src/chk3/diagnostics_collection --
 *   Class used to implement the collection api where
 *   all reported diagnostics are collected.
 *   This is used by the custom diagnostic consumer passed to the clang APIs.
 *   At the time of writing, the stored information about diagnostics are the
 *   followings:
 *      - the subsystem (check) that generated the diagnostic
 *      - the file the diagnostic was generated in
 *      - the line/column information about the diagnostic
 *      - the text of the diagnostic
 *      - (if possible) the area around the generated diagnostic
 *      - (if applicable) the relevant identifier
 *
 *   Other than this, each note level diagnostic is "chained" to the previous diagnostic
 *   generated.
 */

#ifndef CG3_DIAGNOSTICS_COLLECTION_HXX
#define CG3_DIAGNOSTICS_COLLECTION_HXX

#include <array>
#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <chk3/checks.hxx>

#include <clang/Basic/Diagnostic.h>
#include <llvm/ADT/SmallVector.h>

namespace cg3 {
    struct diagnostic {
        diagnostic(clang::DiagnosticsEngine::Level level,
                   std::filesystem::path filename,
                   unsigned line,
                   unsigned column,
                   std::string output_text,
                   std::string relevant_code)
             : level(level),
               filename(std::move(filename)),
               line(line),
               column(column),
               output_text(std::move(output_text)),
               relevant_code(std::move(relevant_code)) { }
        diagnostic(clang::DiagnosticsEngine::Level level,
                   std::filesystem::path filename,
                   unsigned line,
                   unsigned column,
                   std::string output_text,
                   std::string relevant_code,
                   std::string relevant_id)
             : level(level),
               filename(std::move(filename)),
               line(line),
               column(column),
               output_text(std::move(output_text)),
               relevant_code(std::move(relevant_code)),
               relevant_identifier(relevant_id) { }

        [[nodiscard]] bool
        has_identifier() const noexcept {
            return relevant_identifier.has_value();
        }

        clang::DiagnosticsEngine::Level level;
        std::filesystem::path filename;
        unsigned line;
        unsigned column;
        std::string output_text;
        std::string relevant_code;
        std::optional<std::string> relevant_identifier{std::nullopt};
    };

    struct diagnostics_collection;

    struct diagnostic_chain {
        [[nodiscard]] constexpr bool
        operator==(diagnostic_chain other) const noexcept {
            return _type == other._type
                   && _diag_position == other._diag_position;
        }
        [[nodiscard]] constexpr bool
        operator!=(diagnostic_chain other) const noexcept {
            return _type != other._type
                   || _diag_position != other._diag_position;
        }

    private:
        friend diagnostics_collection;

        constexpr explicit diagnostic_chain(cg3::check_types type, std::size_t diag_pos)
             : _type(type),
               _diag_position(diag_pos) { }

        cg3::check_types _type;
        std::size_t _diag_position;
    };

    struct chain_iterator;

    struct diagnostics_collection {
        using diag_chain_type = llvm::SmallVector<diagnostic, 1>; // optimize for the case that diags are not chained

        [[nodiscard]] diagnostic_chain
        new_chain(check_types type);

        diagnostic_chain
        chain_diagnostic(diagnostic_chain chain, const diagnostic& diag);

        [[nodiscard]] diagnostic_chain
        chain_diagnostic(check_types type, const diagnostic& diag);

        [[nodiscard]] std::size_t
        chain_count() const;

        [[nodiscard]] constexpr std::size_t
        type_count() const noexcept { return _diagnostics.size(); }

        [[nodiscard]] chain_iterator
        begin_chains();
        [[nodiscard]] chain_iterator
        end_chains();

    private:
        friend chain_iterator;
        using chain_store = std::vector<diag_chain_type>;
        std::array<chain_store,
                   static_cast<std::size_t>(check_types::COUNT)>
               _diagnostics{};
    };

    inline constexpr bool
    operator== [[nodiscard]] (const cg3::chain_iterator& self, const cg3::chain_iterator& other);
    inline constexpr bool
    operator!= [[nodiscard]] (const cg3::chain_iterator& self, const cg3::chain_iterator& other);

    struct chain_iterator {
        using difference_type = std::ptrdiff_t;
        using value_type = diagnostics_collection::diag_chain_type;
        using pointer = std::add_pointer_t<value_type>;
        using reference = std::add_lvalue_reference_t<value_type>;
        using iterator_category = std::forward_iterator_tag;

        chain_iterator() = default;

        chain_iterator&
        operator++() {
            increase_step();
            return *this;
        }
        const chain_iterator
        operator++(int) {
            auto cpy = *this;
            increase_step();
            return cpy;
        }

        reference
        operator*() noexcept {
            assert(!is_after_end());

            return _collection->_diagnostics[_type_cnt][_chain_cnt];
        }
        const value_type&
        operator*() const noexcept {
            assert(!is_after_end());

            return _collection->_diagnostics[_type_cnt][_chain_cnt];
        }
        [[nodiscard]] pointer
        operator->() noexcept {
            assert(!is_after_end());

            return &_collection->_diagnostics[_type_cnt][_chain_cnt];
        }
        [[nodiscard]] std::add_const_t<value_type>*
        operator->() const noexcept {
            assert(!is_after_end());

            return &_collection->_diagnostics[_type_cnt][_chain_cnt];
        }

        friend constexpr bool
        operator==(const chain_iterator& self, const chain_iterator& other);

        friend constexpr bool
        operator!=(const chain_iterator& self, const chain_iterator& other);

    private:
        friend diagnostics_collection;

        explicit chain_iterator(diagnostics_collection* collection)
             : _collection(collection) {
            while (_collection->_diagnostics[_type_cnt].empty()) {
                ++_type_cnt;
                if (_type_cnt == _collection->type_count()) break; // after end reached
            }
        }

        [[nodiscard]] constexpr bool
        is_after_end() const noexcept {
            // an iterator could be after the end by being constructed
            // without a collection (default), or by going after
            // the end (thus type_cnt eq the collection's diag size)
            return _collection == nullptr
                   || _type_cnt == _collection->type_count();
        }

        void
        increase_step() {
            assert(!is_after_end());

            if (_collection->_diagnostics[_type_cnt].size() - 1 > _chain_cnt) {
                ++_chain_cnt;
                return;
            }

            _chain_cnt = 0;
            do {
                ++_type_cnt;
                if (_type_cnt == _collection->type_count()) break; // after end reached
            } while (_collection->_diagnostics[_type_cnt].empty());
        }

        std::size_t _type_cnt{};
        std::size_t _chain_cnt{};
        diagnostics_collection* _collection{};
    };

    inline chain_iterator
    diagnostics_collection::begin_chains() {
        return chain_iterator(this);
    }

    inline chain_iterator
    diagnostics_collection::end_chains() { // NOLINT(readability-convert-member-functions-to-static)
        return {};
    }

    inline constexpr bool
    operator==(const cg3::chain_iterator& self, const cg3::chain_iterator& other) {
        // same collection or one of them is null
        assert(self._collection == other._collection
               || self._collection == nullptr
               || other._collection == nullptr);

        // if our after end status is not equal,
        // then one of os is after the end, while the other is not
        // in this case, we are for sure not equal
        if (self.is_after_end() != other.is_after_end()) return false;
        // we are either both after or before the end
        // if I am after the end then the other is as well, therefore
        // we are equal
        if (self.is_after_end()) return true;

        // after end status did not reveal equivalence, resort to
        // both before end and the numerical values
        return self._type_cnt == other._type_cnt
               && self._chain_cnt == other._chain_cnt;
    }

    inline constexpr bool
    operator!=(const chain_iterator& self, const chain_iterator& other) {
        return !(self == other);
    }
}

#endif
