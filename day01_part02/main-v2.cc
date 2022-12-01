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
#include <optional>

template<typename Stream>
struct read_elf_calories
{
    Stream& stream_;

    std::optional<int> operator()()
    {
        if (stream_.eof()) return {};

        return {
            std::accumulate(
                laparca::read_line_until_empty_iterator<int, Stream>(std::cin),
                laparca::read_line_until_empty_iterator<int, Stream>{},
                0
            )
        };
    }
};
template<typename Stream>
read_elf_calories(Stream&) -> read_elf_calories<Stream>;

int main()
{
    auto reader = read_elf_calories{std::cin};
    std::vector<int> elf_calories{
	    laparca::optional_generator_iterator<int>(reader),
            laparca::optional_generator_iterator<int>()};

    std::ranges::sort(elf_calories, std::greater{});

    auto top_three = elf_calories | std::ranges::views::take(3);
    auto sum = std::accumulate(std::begin(top_three), std::end(top_three), 0);
    
    std::cout << "The sum of top three elfs by calories is " << sum << std::endl;

    return 0;
}

