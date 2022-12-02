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
#include <iterators.hh>
#include <ranges>
#include <numeric>
#include <algorithm>

int main()
{
    using iterator = laparca::read_line_iterator<decltype(std::cin)>;

    std::vector<std::string> input{
        iterator{std::cin},
        iterator{}
    };
    
    auto sum_elf_calories = [](const auto& values)
    {
        auto v = values | std::views::common;
        return std::accumulate(v.begin(), v.end(), 0, [](int prev, const std::string& calories) {
            return prev + std::stoi(calories);
        });
    };

    auto elf_calories_list = input | std::views::split(std::string{""}) 
                                   | std::views::transform(sum_elf_calories)
                                   ;

    std::vector<int> elf_calories{
        std::ranges::begin(elf_calories_list),
        std::ranges::end(elf_calories_list)
    };

    std::ranges::sort(elf_calories, std::greater{});

    std::cout << "Max calories by one elf is " << *elf_calories.begin() << "; Max calories by top threee elfs is " << std::accumulate(elf_calories.begin(), elf_calories.begin() + 3, 0) << std::endl;
    
    return 0;
}

