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
#include <algorithm.hh>
#include <conversions.hh>
#include <aoc/point.hh>

bool is_digit(char c)
{
    return (c >= '0' && c <= '9') || c == '-';
}

struct point_from_vector_t
{
    using is_algorithm = std::true_type;

    template<typename C>
    requires laparca::algorithm::is_container<C>
    point<int64_t> operator()(C&& v) const
    {
        return {v[0], v[1]};
    }
} point_from_vector;

int manhattan_distance(const point<int64_t>& a, const point<int64_t>& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

int main(int argc, char** argv)
{
    int row_to_check  = 2000000;
    int64_t frec_multiply = 4000000;

    if (argc >= 2)
    {
        if (std::string(argv[1]) == "--test")
        {
            row_to_check  = 10;
            frec_multiply = 20;
        }
    }
    auto pairs = std::string{std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>()} |
        laparca::trans::split('\n') |
        laparca::trans::map(
            laparca::trans::split(':') |
            laparca::trans::map(
                laparca::trans::split(',') |
                laparca::trans::map(
                    laparca::trans::filter(is_digit) |
                    laparca::str_converter<int>
                ) |
                point_from_vector
            )
        )
        ;
    
    auto top_left = pairs[0][0];
    auto bottom_right = pairs[0][0];

    pairs | laparca::trans::for_each(
        laparca::trans::for_each([&top_left, &bottom_right](const point<int64_t>& p)
        {
            if (p.x > bottom_right.x)
                bottom_right.x = p.x;
            if (p.x < top_left.x)
                top_left.x = p.x;
            if (p.y > bottom_right.y)
                bottom_right.y = p.y;
            if (p.y < top_left.y)
                top_left.y = p.y;
        })
    );

    auto distances = pairs | laparca::trans::map([](const std::vector<point<int64_t>>& v) { return manhattan_distance(v[0], v[1]); });
    auto max_distance = distances | laparca::trans::sort(laparca::functional::greater) | laparca::trans::first;

    int64_t count = 0;
    for (int64_t x = top_left.x - max_distance; x < bottom_right.x + max_distance; ++x)
    {
        const point<int64_t> p{x, row_to_check};
        for (size_t i = 0; i < pairs.size(); i++)
        {
            const auto& sensor = pairs[i][0];
            const auto& beacon = pairs[i][1];
            if (p != sensor && p != beacon && manhattan_distance(p, sensor) <= distances[i])
            {
                count ++;
                break;
            }
        }
    }

    std::cout << "Result part 1 = " << count << std::endl;

    int continue_searching = true;
    for (int64_t y = 0; y < frec_multiply && continue_searching; ++y)
        for (int64_t x = 0; x < frec_multiply && continue_searching; ++x)
        {
            point<int64_t> p{x, y};
            bool occupied = false;

            for (size_t i = 0; i < pairs.size(); i++)
            {
                const auto& sensor = pairs[i][0];
                const auto& beacon = pairs[i][1];
                if (p == sensor || p == beacon || manhattan_distance(p, sensor) <= distances[i])
                {
                    int new_x = distances[i] - std::abs(sensor.y - p.y) + sensor.x;
                    if (new_x > x+1)
                        x = new_x;
                    occupied = true;
                    break;
                }
            }

            if (not occupied)
            {
                std::cout << "Result Part 2 = " << (p.x * frec_multiply + p.y) << " " << p << std::endl;
                continue_searching = false;
            }
        }
    return 0;
}