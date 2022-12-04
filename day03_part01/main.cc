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
#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <functional>

template<typename It>
It search_reapeted(It start1, It end1, It start2, It end2)
{
    while(start1 != end1 && start2 != end2)
    {
        if (*start1 == *start2) return start1;
        if (*start1 > *start2) start2++;
        else start1++;
    }

    return end1;
}

int item_priority(char v)
{
    if (v >= 'a' && v <= 'z') return v - 'a' + 1;
    if (v >= 'A' && v <= 'Z') return v - 'A' + 27;
    return 0;
}

int main()
{
    int total_priority = 0;
    while(!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);
        std::cout << "-----" << std::endl;
        std::cout << line << std::endl;

        auto start = std::begin(line);
        auto end = std::end(line);
        auto middle = start + (end - start) / 2;

        std::for_each(start, middle, [](auto a){ std::cout << a;});
        std::cout << std::endl;
        std::for_each(middle, end, [](auto a){ std::cout << a;});
        std::cout << std::endl;

        std::ranges::sort(start, middle);
        std::ranges::sort(middle, end);
        std::cout << line << std::endl;
        std::cout << item_priority(*search_reapeted(start, middle, middle, end)) << std::endl;
        total_priority += item_priority(*search_reapeted(start, middle, middle, end));
    }

    std::cout << "total priority = " << total_priority << std::endl;

    return 0;
}

