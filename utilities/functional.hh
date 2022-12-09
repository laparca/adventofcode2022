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
        struct plus
        {
            template<typename A, typename B>
            auto operator()(A&& a, B&& b)
            {
                return a + b;
            }
        };

        struct minus
        {
            template<typename A, typename B>
            auto operator()(A&& a, B&& b)
            {
                return a - b;
            }
        };

        struct mult
        {
            template<typename A, typename B>
            auto operator()(A&& a, B&& b)
            {
                return a * b;
            }
        };
        
        struct div
        {
            template<typename A, typename B>
            auto operator()(A&& a, B&& b)
            {
                return a / b;
            }
        };

        struct less
        {
            template<typename A, typename B>
            bool operator()(A&& a, B&& b)
            {
                return a < b;
            }
        };
        
        struct less_eq
        {
            template<typename A, typename B>
            bool operator()(A&& a, B&& b)
            {
                return a <= b;
            }
        };

        struct greater
        {
            template<typename A, typename B>
            bool operator()(A&& a, B&& b)
            {
                return a > b;
            }
        };
        
        struct greater_eq
        {
            template<typename A, typename B>
            bool operator()(A&& a, B&& b)
            {
                return a >= b;
            }
        };

        struct equal_to
        {
            template<typename A, typename B>
            bool operator()(A&& a, B&& b)
            {
                return a == b;
            }
        };
        
        struct distinct_to
        {
            template<typename A, typename B>
            bool operator()(A&& a, B&& b)
            {
                return a == b;
            }
        };
    }
}
