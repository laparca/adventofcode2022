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
#include <string>
#include <iostream>
#include <aoc/point.hh>
#include <algorithm.hh>
#include <conversions.hh>

struct parse_point
{
    point operator()(const std::string& str)
    {
        auto values = str
            | laparca::trans::split(',')
            | laparca::trans::map(laparca::str_converter<int>)
            ;
        return {values[0], values[1]};
    }
};

constexpr point start{500, 0};

std::tuple<std::vector<std::string>, point, point> prepare_canvas(const std::vector<std::vector<point>>& points)
{
    // Get min, max bounds for canvas        
    auto plain_pairs = points | laparca::trans::concat();
    plain_pairs.push_back(start);
    const auto order_horizontal = plain_pairs | laparca::trans::sort([](const point& v1, const point& v2) { return v1.x < v2.x; });
    const auto order_vertical   = plain_pairs | laparca::trans::sort([](const point& v1, const point& v2) { return v1.y < v2.y; });
    const point top_left    {order_horizontal[0                        ].x, order_vertical[0                      ].y};
    const point bottom_right{order_horizontal[order_horizontal.size()-1].x, order_vertical[order_vertical.size()-1].y};
    const point size = bottom_right - top_left + point{1, 1};

    std::cout << top_left << ", " << bottom_right <<  ", " << (bottom_right - top_left)<< std::endl;
    
    std::vector<std::string> canvas(size.y);

    for (auto& str : canvas)
        str = std::string(size.x, ' ');


    for (auto& point_chain : points)
    {
        point from = point_chain[0];
        std::for_each(std::begin(point_chain) + 1, std::end(point_chain), [top_left, &from, &canvas](const point& to)
        {
            point desp = (to - from).sgn();
            point act = from;
            while(true)
            {
                canvas[act.y][act.x - top_left.x] = '#';
                if (act == to)
                    break;
                act += desp;
            };
            from = to;
        });
    }
    return {canvas, top_left, bottom_right};
}

char get_value(const std::vector<std::string>& canvas, const point& position)
{
    /* by default, out of bound is an empty location */
    if (position.y < 0 || position.x < 0 || position.y >= (int)canvas.size() || position.x >= (int)canvas[0].size())
        return ' ';

    return canvas[position.y][position.x];
}

void set_value(std::vector<std::string>& canvas, const point& position, const char val)
{
    canvas[position.y][position.x] = val;
}

bool is_valid(const point& p, const size_t max_x, const size_t max_y)
{
    return p.x >= 0 && p.x < (int)max_x && p.y >= 0 && p.y < (int)max_y;
}
/**
 * @brief 
 * 
 * @param canvas 
 * @param top_left 
 * @return true if the sand can be put
 * @return false if the sand got out of the canvas
 */
bool put_sand_unit(std::vector<std::string>& canvas, const point& top_left)
{
    const point correction{-top_left.x, 0};
    const point down[] = {
        { 0, 1}, /* down */
        {-1, 1}, /* down_left */
        { 1, 1}  /* down_right */
    };
    const size_t width = canvas[0].size();
    const size_t height = canvas.size();
    const point the_begining{start + correction};
    constexpr point invalid{-1, -1};

    if (get_value(canvas, the_begining) != ' ') 
        return false;

    for (point act{the_begining}; is_valid(act, width, height);)
    {
        point next{invalid};
        for (const auto& direction : down)
        {
            if (get_value(canvas, act + direction) == ' ')
            {
                next = act + direction;
                break;
            }
        }

        if (next == invalid)
        {
            set_value(canvas, act, 'o');
            return true;
        }

        act = next;
    }

    return false;
}

void show_canvas(const std::vector<std::string>& canvas)
{
    for (const auto& line : canvas)
        std::cout << line << std::endl;
}
int main()
{
    using namespace std::string_literals;

    auto pairs = std::string{std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>()}
        | laparca::trans::filter([](char a){ return a!=' ' && a != '>'; })
        | laparca::trans::convert_to<std::basic_string>()
        | laparca::trans::split('\n')
        | laparca::trans::map(laparca::trans::split('-') | laparca::trans::map(parse_point{}))
        ;
    point tl, br;
    {
        auto [canvas, top_left, bottom_right] = prepare_canvas(pairs);
        int cont = 0;
        while(put_sand_unit(canvas, top_left))
            ++cont;
        show_canvas(canvas);
        std::cout << "Result part 1 = " << cont << std::endl;

        tl = top_left;
        br = bottom_right;
    }

    /* Prepare Part 2 scenario */
    /* we have to calculate an "infinite" bottom. 4 times the height will be enougth */
    {
        const point size{br - tl};
        const point start{tl.x - size.y, br.y + 2};
        const point end  {br.x + size.y, br.y + 2};
        std::vector<point> bottom{start, end};
        pairs.push_back(bottom);

        auto [canvas_pt2, top_left_pt2, bottom_right_pt2] = prepare_canvas(pairs);
        int cont_pt2 = 0;
        while (put_sand_unit(canvas_pt2, top_left_pt2))
            ++cont_pt2;
        show_canvas(canvas_pt2);
        std::cout << "Result part 2 = " << cont_pt2 << std::endl;
    }

    return 0;
}