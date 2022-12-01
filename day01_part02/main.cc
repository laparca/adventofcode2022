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

template<typename Stream>
int read_elf_calories(Stream& stream)
{
    std::string str;
    int calories_sum = 0;

    if (stream.eof()) return {};

    while(!stream.eof())
    {
        std::getline(stream, str);
        if (str == "")
            break;
        calories_sum += std::stoi(str);
    }

    return calories_sum;
}

int main()
{
    std::vector<int> elf_calories;

    while(!std::cin.eof())
    {
        elf_calories.emplace_back(read_elf_calories(std::cin));
    }

    std::ranges::sort(elf_calories, std::greater{});

    auto top_three = elf_calories | std::ranges::views::take(3);
    auto sum = std::accumulate(std::begin(top_three), std::end(top_three), 0);
    
    std::cout << "The sum of top three elfs by calories is " << sum << std::endl;

    return 0;
}

