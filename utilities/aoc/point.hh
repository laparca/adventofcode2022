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
#include <math.hh>

template<typename T=int>
struct point
{
    T x;
    T y;

    point sgn() {
        return {
            laparca::sgn(x),
            laparca::sgn(y)
        };
    }
};
template<typename T>
point(T, T) -> point<T>;

template<typename T>
bool operator==(const point<T>& a, const point<T>& b)
{
    return a.x == b.x && a.y == b.y;
}

template<typename T>
bool operator!=(const point<T>& a, const point<T>& b)
{
    return !(a == b);
}

template<typename T>
point<T> operator+(const point<T>& a, const point<T>& b)
{
    return {a.x+b.x, a.y+b.y};
}

template<typename T>
point<T>& operator+=(point<T>& a, const point<T>& b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

template<typename T>
point<T> operator-(const point<T>& a, const point<T>& b)
{
    return {a.x-b.x, a.y-b.y};
}

template<typename T>
point<T>& operator-=(point<T>& a, const point<T>& b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

template<typename T>
float distance(const point<T>& a, const point<T>& b)
{
    return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

template<typename T>
std::ostream& operator<<(std::ostream& s, const point<T>& p)
{
    return (s << "{ x: " << p.x << ", y: " << p.y << " }");
}
