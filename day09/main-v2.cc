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
#include <map>
#include <algorithm.hh>

/* Based in the solution from Dxr: https://t.me/OfftopicSince2017/282897 */

struct point
{
    int x = 0;
    int y = 0;

    point sgn()
    {
        return {
            x > 0 ? 1 : x < 0 ? -1 : 0,
            y > 0 ? 1 : y < 0 ? -1 : 0
        };
    }
};

point operator+(const point& p1, const point& p2)
{
    return {p1.x + p2.x, p1.y + p2.y};
}

point operator-(const point& p1, const point& p2)
{
    return {p1.x - p2.x, p1.y - p2.y};
}
bool operator<(const point& p1, const point& p2)
{
    return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
}
bool operator!=(const point& p1, const point& p2)
{
    return p1.x != p2.x || p1.y != p2.y;
}

std::map<std::string, point> movement {
    {"D", { 0,  1}},
    {"U", { 0, -1}},
    {"L", {-1,  0}},
    {"R", { 1,  0}}
};

#include <io_util.hh>

int main()
{
    constexpr size_t positions = 10;
    std::array<point, positions> rope;
    std::set<point> visited_second;
    std::set<point> visited_nineth;

    visited_second.insert(rope[1]);
    visited_nineth.insert(rope[9]);

    laparca::istream_to_container{std::cin}
        | laparca::trans::transform(laparca::trans::split(' '))
        | laparca::trans::for_each(
              [&visited_second, &visited_nineth, &rope](const std::vector<std::string>& values) {
                  for (int i = 0; i < std::stol(values[1]); ++i)
                  {
                      rope[0] = rope[0] + movement[values[0]];

                      for (int i = 1; i < 10; i++)
                      {
                          auto new_pos = rope[i] + (rope[i-1] - rope[i]).sgn();
                          if (new_pos != rope[i-1])
                              rope[i] = new_pos;
                      }
                      visited_second.insert(rope[1]);
                      visited_nineth.insert(rope[9]);
                  }
              }
          )
        ;

    std::cout << "Positions visited by tail = " << visited_second.size() << std::endl;
    std::cout << "Positions visited by end of rope = " << visited_nineth.size() << std::endl;
    return 0;
}
