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
#include <vector>
#include <deque>
//#include <iterators.hh>
#include <algorithm>
#include <sstream>

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

int main()
{
    using stack_crate = std::deque<char>;
    std::vector<stack_crate> stacks;

    while(!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);

        auto line_start = std::begin(line);
        auto line_end   = std::end(line);
        auto cargo = std::find(line_start, line_end, '[');
        if (cargo != line_end)
        {
            do
            {
                size_t stack_number = (cargo - line_start) / 4;
                cargo = std::next(cargo);

                while (stacks.size() < (stack_number+1))
                    stacks.emplace_back();

                stacks[stack_number].push_back(*cargo);
                cargo = std::find(cargo, line_end, '[');
            } while(cargo != line_end);
        }


        if (cargo == line_end && line.starts_with("move"))
        {
            auto tokens = split(line, ' ');
            int quantity = std::stoi(tokens[1]);
            int from = std::stoi(tokens[3])-1;
            int to = std::stoi(tokens[5])-1;

            for (; quantity; --quantity)
            {
                stacks[to].push_front(stacks[from].front());
                stacks[from].pop_front();
            }
        }
    }

    for (auto s : stacks)
    {
        std::cout << s.front();
    }

    std::cout << std::endl;
    return 0;
}
