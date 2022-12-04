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

bool is_contained(std::vector<int> a, std::vector<int> b)
{
    return a[0] >= b[0] && a[1] <= b[1];
}

int main()
{
    int count = 0;

    for (auto line : std::cin)
    {
        auto sections = split(line, ',');

        auto section1_str = split(sections[0], '-');
        auto section2_str = split(sections[1], '-');

        std::vector<int> section1;
        std::vector<int> section2;

        std::transform(std::begin(section1_str), std::end(section1_str),
                       std::back_insert_iterator(section1), [](auto& v) { return std::stoi(v); });
        std::transform(std::begin(section2_str), std::end(section2_str),
                       std::back_insert_iterator(section2), [](auto& v) { return std::stoi(v); });

        if (is_contained(section1, section2) || is_contained(section2, section1))
            count++;
    }

    std::cout << "sections fully contained count is " << count << std::endl;
    return 0;
}
