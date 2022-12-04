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
#include <ranges>
#include <algorithm>
#include <iterators.hh>
#include <sstream>
namespace std
{
    laparca::read_line_iterator<istream> begin(istream& stream)
    {
        return {stream};
    }

    laparca::read_line_iterator<istream> end(istream&)
    {
        return {};
    }
}

/* From: https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/ */
std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

struct elf_range
{
    int start;
    int end;

    bool contains(int i)
    {
        return i >= start && i <= end;
    }

    bool contains(const elf_range& r)
    {
        return contains(r.start) && contains(r.end);
    }

    bool overlaps(const elf_range& r)
    {
        return contains(r.start) || contains(r.end);
    }
};

elf_range from_string(const std::string& values)
{
    const auto pair = split(values, '-');
    return elf_range{
        std::stoi(pair[0]),
        std::stoi(pair[1])
    };
}

int main()
{
    int count = 0;
    int count_overlaps = 0;

    for (auto line : std::cin)
    {
        auto sections = split(line, ',');
        auto range_1 = from_string(sections[0]);
        auto range_2 = from_string(sections[1]);

        if (range_1.contains(range_2) || range_2.contains(range_1))
            count++;
        if (range_1.overlaps(range_2) || range_2.overlaps(range_1))
            count_overlaps++;
    }

    std::cout << "sections fully contained count is " << count << std::endl;
    std::cout << "total overlaps is " << count_overlaps << std::endl;
    return 0;
}
