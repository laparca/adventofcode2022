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
#include <io_util.hh>
#include <algorithm.hh>
#include <cmath>
#include <iostream>
#include <deque>
#include <aoc/point.hh>

struct visited_point
{
    point<int> p;
    int steps;
    char elevation;
};

bool operator==(const visited_point& a, const visited_point& b)
{
    return a.p == b.p && a.steps == b.steps;
}
bool operator!=(const visited_point& a, const visited_point& b)
{
    return !(a == b);
}
bool operator<(const visited_point& a, const visited_point& b)
{
    return a.p == b.p && a.steps < b.steps;
}
bool operator<=(const visited_point& a, const visited_point& b)
{
    return a.p == b.p && a.steps <= b.steps;
}
bool operator>(const visited_point& a, const visited_point& b)
{
    return a.p == b.p && a.steps > b.steps;
}
std::ostream& operator<<(std::ostream& s, const visited_point& p)
{
    return (s << "{ p: " << p.p << ", steps: " << p.steps << ", elevation: '" << p.elevation << "' }");
}

using terrain_map = std::vector<std::string>;
std::tuple<terrain_map, point<int>, point<int>> prepare_data(const terrain_map& original_map)
{
    const int width = original_map[0].size();
    const int height = original_map.size();

    terrain_map prepared_map{original_map};
    point<int> start{0, 0};
    point<int> end{0, 0};


    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            switch(original_map[y][x])
            {
                case 'S':
                    start = point{x, y};
                    prepared_map[y][x] = 'a';
                    break;
                case 'E':
                    end = point{x, y};
                    prepared_map[y][x] = 'z';
                    break;
                default:
                    break;
            }
        }
    
    return {prepared_map, start, end};
}

char get_value(const terrain_map& map, const point<int>& p)
{
    return map[p.y][p.x];
}

bool is_valid_difference_reverse(char from, char to)
{
    if (from <= to) return true;
    return from - to <=1;
}
std::vector<point<int>> calculate_movements_reverse(const point<int>& act, const terrain_map& map)
{
    const int width = map[0].size();
    const int height = map.size();
    const char height_start = map[act.y][act.x];

    const point<int> up    = act + point{ 0, -1};
    const point<int> down  = act + point{ 0,  1};
    const point<int> left  = act + point{-1,  0};
    const point<int> right = act + point{ 1,  0};

    std::vector<point<int>> next_movements;

    if (act.y != 0 && is_valid_difference_reverse(height_start, get_value(map, up)))
        next_movements.emplace_back(up);
    if (act.y != height - 1 && is_valid_difference_reverse(height_start, get_value(map, down)))
        next_movements.emplace_back(down);
    if (act.x != 0 && is_valid_difference_reverse(height_start, get_value(map, left)))
        next_movements.emplace_back(left);
    if (act.x != width - 1 && is_valid_difference_reverse(height_start, get_value(map, right)))
        next_movements.emplace_back(right);
    
    return next_movements;
}

template<typename Func>
std::tuple<int, std::vector<visited_point>> sortest_path(const point<int>& start, const point<int>& end, const std::vector<std::string>& map, Func movements)
{
    std::vector<visited_point> visited{ };
    std::deque<visited_point> candidates{ {start, 0, map[start.y][start.x]} };

    while(candidates.size())
    {
        visited_point act = candidates.front();
        candidates.pop_front();

        auto it = std::find_if(std::begin(visited), std::end(visited), [act](const auto& a){ return act.p == a.p; });
        if (it == std::end(visited))
            visited.emplace_back(act);
        else if (*it > act)
            *it = act;
        
        if (act.p == end)
            continue;

        auto values = movements(act.p, map);

        auto sorted_values = values | laparca::trans::sort([end](const point<int>& a, const point<int>& b)
                                                           { return distance(a, end) < distance(b, end); });
        for (const auto &candidate : sorted_values)
        {
            bool is_candiate = true;
            visited_point visited_candidate{candidate, act.steps + 1, map[candidate.y][candidate.x]};


            for (const auto &visited : visited)
            {
                if (visited <= visited_candidate)
                {
                    is_candiate = false;
                    break;
                }
            }

            for (const auto& future: candidates)
            {
                if (future <= visited_candidate)
                {
                    is_candiate = false;
                    break;
                }
            }

            if (is_candiate)
                candidates.emplace_back(std::move(visited_candidate));
        }
    }

#if 0
    auto order_visited = visited | laparca::trans::sort([](const visited_point& a, const visited_point& b) {
        return b.elevation < a.elevation || (b.elevation == a.elevation && b.steps > a.steps);
    }) | laparca::trans::for_each([](const auto& v) { std::cout << v << ", ";});
    std::cout << std::endl;
    std::cout << "visited " << order_visited.size() << std::endl;

    auto new_map = map;
    for(const auto& v: visited)
    {
        new_map[v.p.y][v.p.x] = ' ';
    }
    for (size_t y = 0; y < map.size(); ++y)
    {
        for (size_t x = 0; x < map[0].size(); ++x)
            if (new_map[y][x] == ' ')
                std::cout << "\033[34m" << map[y][x] << "\033[0m";
            else
                std::cout << map[y][x];
        std::cout << std::endl;
    }
#endif
    auto results = visited | laparca::trans::filter([end](const visited_point& p) {
        return p.p == end;
    });
    if (results.size() == 0) return {999, {}};
    return {results[0].steps, visited};
}

int main()
{
    auto input = std::string{std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>()} | laparca::trans::split('\n');
    auto [map, start, end] = prepare_data(input);

    auto [steps, visited] = sortest_path(end, start, map, calculate_movements_reverse);
    std::cout << "Part 1 result: " << steps << std::endl;

    auto minor_a = (visited | laparca::trans::sort([](const visited_point& a, const visited_point& b) {
        return a.elevation < b.elevation || (a.elevation == b.elevation && a.steps < b.steps);
    }))[0];
    std::cout << "Part 2 result: " << minor_a.steps << std::endl;
    return 0;
}