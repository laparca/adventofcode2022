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
#include <numeric>
#include <vector>
#include <cmath>
#include <set>
#include <sstream>
#include <array>

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

struct position
{
    int x;
    int y;

    position move(char direction)
    {
        switch(direction)
        {
            case 'D':
                return position{x, y+1};
            case 'U':
                return position{x, y-1};
            case 'L':
                return position{x-1, y};
            case 'R':
                return position{x+1, y};
        }
        return *this;
    }

    position follow(const position& following)
    {
        if (is_next(following))
            return *this;
        /* If same row or column, you should only move 1 position */
        if (x == following.x)
            return position{x, (y + following.y)/2 };
        if (y == following.y)
            return position{(x + following.x)/2, y};
        /* Other case, should move in diagonal */
        return position{
            x + (x > following.x ? -1 : 1),
            y + (y > following.y ? -1 : 1)
        };
    }

    bool operator<(const position& p) const
    {
        return x < p.x || (x == p.x && y < p.y);
    }
    bool is_next(const position& p) const
    {
        return std::abs(x - p.x) <= 1 && std::abs(y - p.y) <= 1;
    }
};

int main()
{
    constexpr size_t positions = 10;
    std::array<position, positions> rope;
    std::set<position> visited_second;
    std::set<position> visited_nineth;

    for (size_t i = 0; i < positions; ++i)
        rope[i] = position{0, 0};

    visited_second.insert(rope[1]);
    visited_nineth.insert(rope[9]);

    while(!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size() == 0) break;

        auto values = split(line, ' ');
        for (int i = 0; i < std::stol(values[1]); ++i)
        {
            rope[0] = rope[0].move(values[0][0]);
            for (int i = 1; i < 10; i++)
                rope[i] = rope[i].follow(rope[i-1]);
            visited_second.insert(rope[1]);
            visited_nineth.insert(rope[9]);
        }
    }

    std::cout << "Positions visited by tail = " << visited_second.size() << std::endl;
    std::cout << "Positions visited by end of rope = " << visited_nineth.size() << std::endl;
    return 0;
}
