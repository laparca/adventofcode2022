/******************************************************************************
 * Copyright (C) 2022 Samuel R. Sevilla <laparca@laparca.es>
 *
 * This file is part of adventofcode2022.
 *
 * adventofcode2022 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 * 
 * adventofcode2022 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Foobar. If not, see <https://www.gnu.org/licenses/>.
 ******************************************************************************/
#pragma once
#include <numeric>
#include <algorithm>
#include <type_traits>
#include <functional>
#include <utility>
#include <memory>
#include <functional.hh>

namespace laparca
{
    namespace algorithm
    {
        template<typename T>
        concept is_algorithm = std::is_same_v<typename T::is_algorithm, std::true_type>;

        template<typename T>
        concept is_container = requires(T v)
        {
            //typename T::value_type;
            std::begin(v);
            std::end(v);
        };

        template<typename Left, typename Right>
        struct join_algorithm
        {
            using is_algorithm = std::true_type;
            using left_type = std::decay_t<Left>;
            using right_type = std::decay_t<Right>;
 
            template<typename C>
            requires is_container<C>
            constexpr auto operator()(C&& v) const
            {
                return right_(left_(std::forward<C>(v)));
            }

            left_type left_;
            right_type right_;
        };

        template<typename Left, typename Right>
        requires is_algorithm<Left> && is_algorithm<Right>
        constexpr join_algorithm<std::decay_t<Left>, std::decay_t<Right>> operator|(Left&& left, Right&& right)
        {
            return {std::forward<Left>(left), std::forward<Right>(right)};
        }

        template<typename Container, typename Algo>
        requires is_container<Container> && is_algorithm<Algo>
        constexpr auto operator|(Container&& c, Algo&& a)
        {
            return a(std::forward<Container>(c));
        }


        template<typename F, template<typename...> class Out/*, template<typename> class Allocator*/>
        struct transform
        {
            using is_algorithm = std::true_type;
            using func_type = std::decay_t<F>;

            template<typename Container>
            requires is_container<Container>
            constexpr auto operator()(Container&& c) const
            {
                using value_type = std::decay_t<std::invoke_result_t<F, typename std::decay_t<Container>::value_type>>;
                /*using allocator_type = Allocator<value_type>;*/

                Out<value_type/*, allocator_type*/> results/*(allocator_type{})*/;

                std::transform(std::begin(c), std::end(c), std::back_inserter(results), func_);

                return results;
            }

            func_type func_;
        };

        template<typename T, template<typename...> class Out/*, template<typename> class Allocator*/>
        struct split
        {
            using is_algorithm = std::true_type;
            using delimiter_type = std::decay_t<T>;

            template<typename Container>
            requires is_container<Container>
            constexpr auto operator()(Container&& s) const
            {
                using value_type = std::decay_t<Container>;
                /*using allocator_type = Allocator<value_type>;*/

                Out<value_type/*, allocator_type*/> tokens/*(allocator_type{})*/;

                auto token_start = std::begin(s);
                auto token_end = std::end(s);
                for(auto it = token_start; it != token_end; ++it)
                {
                    if (*it == delimiter_)
                    {
                        if (it != token_start || split_contiguous_)
                            tokens.emplace_back(token_start, it);
                        token_start = std::next(it);
                    }
                }

                if (token_start != token_end)
                    tokens.emplace_back(token_start, token_end);
                
                return tokens;
            }
            
            delimiter_type delimiter_;
            bool split_contiguous_ = false;
        };

        template<typename T, typename Func>
        struct fold
        {
            using is_algorithm = std::true_type;
            using value_type = std::decay_t<T>;
            using func_type = std::decay_t<Func>;

            template<typename C>
            requires is_container<C>
            constexpr auto operator()(C&& container) const
            {
                return accumulate(std::begin(container), std::end(container), initial_, func_);
            }

            value_type initial_;
            func_type func_;
        };

        template<typename Func, template<typename...> class Out/*, template<typename> class Allocator*/>
        struct sort
        {
            using is_algorithm = std::true_type;
            using func_type = std::decay_t<Func>;

            template<typename C>
            requires is_container<C>
            constexpr auto operator()(C&& container) const
            {
                using value_type = typename std::decay_t<C>::value_type;
                /*using allocator_type = Allocator<value_type>;*/

                Out<value_type/*, allocator_type*/> sorted{std::begin(container), std::end(container)/*, allocator_type{}*/};

                std::sort(std::begin(sorted), std::end(sorted), func_);

                return sorted;
            }

            func_type func_;
        };


        template<template<typename...> class NewContainer/*, template<typename> class Allocator*/>
        struct convert
        {
            using is_algorithm = std::true_type;

            template<typename Container>
            requires is_container<Container>
            constexpr auto operator()(Container&& container) const
            {
                using value_type = typename std::decay_t<Container>::value_type;
                /*using allocator_type = Allocator<value_type>;*/
                return NewContainer<value_type/*, allocator_type*/>{std::begin(container), std::end(container)/*, allocator_type{}*/};
            }
        };

        template<typename Func>
        struct for_each
        {
            using is_algorithm = std::true_type;

            template<typename Container>
            requires is_container<Container>
            constexpr auto operator()(Container&& container) const
            {
                for (const auto& v : container)
                {
                    func_(v);
                }

                return container;
            }

            std::decay_t<Func> func_;
        };
        
        template<typename Func, template<typename...> class Out/*, template<typename> class Allocator*/>
        struct filter
        {
            using is_algorithm = std::true_type;
            using func_type = std::decay_t<Func>;

            template<typename C>
            requires is_container<C>
            constexpr auto operator()(C&& container) const
            {
                using value_type = typename std::decay_t<C>::value_type;
                /*using allocator_type = Allocator<value_type>;*/
                using result_type = Out<value_type/*, allocator_type*/>;
                
                result_type result;
                for (const auto& value : container)
                {
                    if (filter_(value))
                        result.emplace_back(value);
                }
                
                return result;
            }

            func_type filter_;
        };

        struct concat
        {
            using is_algorithm = std::true_type;

            template<typename C>
            requires is_container<C> && is_container<typename std::decay_t<C>::value_type>
            constexpr auto operator()(C&& container) const
            {
                using value_type = typename std::decay_t<C>::value_type;

                value_type r;
                for (const auto& value : container)
                    std::copy(std::begin(value), std::end(value), std::back_inserter(r));
                return r;
            }
        };
    }

    namespace trans
    {
        template<typename Func>
        laparca::algorithm::transform<std::decay_t<Func>, std::vector/*, std::allocator*/> transform(Func&& func)
        {
            return {std::forward<Func>(func)};
        }

        template<template<typename...> class Out, typename Func>
        laparca::algorithm::transform<std::decay_t<Func>, Out/*, std::allocator*/> transform(Func&& func)
        {
            return {std::forward<Func>(func)};
        }
        
        template<typename Func>
        laparca::algorithm::transform<std::decay_t<Func>, std::vector/*, std::allocator*/> map(Func&& func)
        {
            return {std::forward<Func>(func)};
        }

        template<template<typename...> class Out, typename Func>
        laparca::algorithm::transform<std::decay_t<Func>, Out/*, std::allocator*/> map(Func&& func)
        {
            return {std::forward<Func>(func)};
        }

        template<typename Delimiter>
        laparca::algorithm::split<std::decay_t<Delimiter>, std::vector/*, std::allocator*/> split(Delimiter&& delimiter, bool split_contiguous = false)
        {
            return {std::forward<Delimiter>(delimiter), split_contiguous};
        }

        template<template<typename...> class Out, typename Delimiter>
        laparca::algorithm::split<std::decay_t<Delimiter>, Out/*, std::allocator*/> split(Delimiter&& delimiter, bool split_contiguous = false)
        {
            return {std::forward<Delimiter>(delimiter), split_contiguous};
        }

        template<typename T, typename Func>
        laparca::algorithm::fold<std::decay_t<T>, std::decay_t<Func>> fold(T&& initial, Func&& func_)
        {
            return {std::forward<T>(initial), std::forward<Func>(func_)};
        }

        template<typename T>
        laparca::algorithm::fold<std::decay_t<T>, laparca::functional::plus_t> accum(T&& initial)
        {
            return {std::forward<T>(initial), laparca::functional::plus};
        }

        laparca::algorithm::sort<laparca::functional::less_t, std::vector/*, std::allocator*/> sort()
        {
            return {laparca::functional::less};
        }

        template<typename Func>
        laparca::algorithm::sort<std::decay_t<Func>, std::vector/*, std::allocator*/> sort(Func&& func)
        {
            return {std::forward<Func>(func)};
        }

        template<template<typename...> class Container>
        laparca::algorithm::convert<Container/*, std::allocator*/> convert_to()
        {
            return {};
        }

        template<typename Func>
        laparca::algorithm::for_each<Func> for_each(Func&& func)
        {
            return {std::forward<Func>(func)};
        }

        template<typename Func>
        laparca::algorithm::filter<Func, std::vector/*, std::allocator*/> filter(Func&& func)
        {
            return {std::forward<Func>(func)};
        }
        
        template<template<typename...> class Out, typename Func>
        laparca::algorithm::filter<Func, Out/*, std::allocator*/> filter(Func&& func)
        {
            return {std::forward<Func>(func)};
        }

        constexpr auto count = []<typename Func>(Func&& func)
        {
            return fold(0, [func](int c, const auto& v) {
                return c + (func(v) ? 1 : 0);
            });
        };

        /**
         * @brief Make a tuple for each elemen of a container with the index on the
         *        original container and the element itself.
         * 
         */
        constexpr auto index = []<template<typename...> class Out = std::vector>(int first_index = 0)
        {
            int start = first_index;
            return map([start](const auto& v) mutable {
                return std::make_tuple(start++, v);
            });
        };

        constexpr laparca::algorithm::concat concat()
        {
            return {};
        }
    }
}
