/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-28.
 *
 * src/cg3-db/path_insert_iterator --
 */
#ifndef CG3_PATH_INSERT_ITERATOR_HXX
#define CG3_PATH_INSERT_ITERATOR_HXX

#include <filesystem>
#include <iterator>
#include <utility>

namespace cg3 {
    template<class Container, class Transformer>
    struct path_insert_iterator {
        using iterator_category = std::output_iterator_tag;
        using value_type = void;
        using pointer = void;
        using reference = void;

        using container_type = Container;
        using difference_type = void;

        path_insert_iterator(Container& cont, const Transformer& tr) noexcept
             : _container(std::addressof(cont)),
               _transformer(tr) { }
        path_insert_iterator(Container& cont, Transformer&& tr) noexcept
             : _container(std::addressof(cont)),
               _transformer(std::move(tr)) { }

        path_insert_iterator&
        operator=(const std::filesystem::path& val) {
            _container->push_back(_transformer(val));
            return *this;
        }

        path_insert_iterator&
        operator=(std::filesystem::path&& val) {
            _container->push_back(_transformer(std::move(val)));
            return *this;
        }

        [[nodiscard]] path_insert_iterator&
        operator*() noexcept {
            return *this;
        }

        path_insert_iterator&
        operator++() noexcept {
            return *this;
        }

        path_insert_iterator
        operator++(int) noexcept {
            return *this;
        }

    private:
        Container* _container;
        Transformer _transformer;
    };

    template<class Cont, class Tf>
    auto
    path_inserter(Cont& c, Tf&& tf) {
        return path_insert_iterator<Cont, std::decay_t<Tf>>(c, std::forward<Tf>(tf));
    }
}

#endif
