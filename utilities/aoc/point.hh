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
#pragma once

#include <cmath>
#include <iostream>

struct point
{
    int x;
    int y;

    point sgn() {
        return {
            x > 0 ? 1 : x < 0 ? -1 : 0,
            y > 0 ? 1 : y < 0 ? -1 : 0
        };
    }
};

bool operator==(const point& a, const point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const point& a, const point& b)
{
    return !(a == b);
}

point operator+(const point& a, const point& b)
{
    return {a.x+b.x, a.y+b.y};
}

point& operator+=(point& a, const point& b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

point operator-(const point& a, const point& b)
{
    return {a.x-b.x, a.y-b.y};
}

point& operator-=(point& a, const point& b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

float distance(const point& a, const point& b)
{
    return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

std::ostream& operator<<(std::ostream& s, const point& p)
{
    return (s << "{ x: " << p.x << ", y: " << p.y << " }");
}
