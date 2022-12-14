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

namespace laparca
{
    namespace functional
    {
        struct plus_t
        {
            template<typename A, typename B>
            constexpr auto operator()(A&& a, B&& b) const
            {
                return a + b;
            }
        };
        constexpr plus_t plus;

        struct minus_t
        {
            template<typename A, typename B>
            constexpr auto operator()(A&& a, B&& b) const
            {
                return a - b;
            }
        };
        constexpr minus_t minus;

        struct mult_t
        {
            template<typename A, typename B>
            constexpr auto operator()(A&& a, B&& b) const
            {
                return a * b;
            }
        };
        constexpr mult_t mult;
        
        struct div_t
        {
            template<typename A, typename B>
            constexpr auto operator()(A&& a, B&& b) const
            {
                return a / b;
            }
        };
        constexpr div_t div;

        struct less_t
        {
            template<typename A, typename B>
            constexpr bool operator()(A&& a, B&& b) const
            {
                return a < b;
            }
        };
        constexpr less_t less;
        
        struct less_eq_t
        {
            template<typename A, typename B>
            constexpr bool operator()(A&& a, B&& b) const
            {
                return a <= b;
            }
        };
        constexpr less_eq_t less_eq;

        struct greater_t
        {
            template<typename A, typename B>
            constexpr bool operator()(A&& a, B&& b) const
            {
                return a > b;
            }
        };
        constexpr greater_t greater;
        
        struct greater_eq_t
        {
            template<typename A, typename B>
            constexpr bool operator()(A&& a, B&& b) const
            {
                return a >= b;
            }
        };
        constexpr greater_eq_t greater_eq;

        struct equal_to_t
        {
            template<typename A, typename B>
            constexpr bool operator()(A&& a, B&& b) const
            {
                return a == b;
            }
        };
        constexpr equal_to_t equal_to;
        
        struct distinct_to_t
        {
            template<typename A, typename B>
            constexpr bool operator()(A&& a, B&& b) const
            {
                return a != b;
            }
        };
        constexpr distinct_to_t distinct_to;

        struct id_t
        {
            template<typename T>
            constexpr T operator()(T&& t) const
            {
                return std::forward<T>(t);
            }
        };
        constexpr id_t id;

        template<size_t N>
        struct get_t
        {
            template<typename T>
            constexpr auto operator()(T&& t) const
            {
                return std::get<N>(std::forward<T>(t));
            }
        };
        template<size_t N>
        constexpr get_t<N> get;
    }
}
