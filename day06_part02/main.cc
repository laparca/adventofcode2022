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

template<typename It>
bool are_repeated(It start, It stop)
{
    for(;start != stop; ++start)
    {
        if (std::find(std::next(start), stop, *start) != stop)
            return true;
    }

    return false;
}
constexpr int width = 14;

int main()
{
    while(!std::cin.eof())
    {
        int count = width;

        std::string line;
        std::getline(std::cin, line);

        auto start = std::begin(line);
        auto stop = std::begin(line) + width;

        while (are_repeated(start, stop))
        {
            start++;
            stop++;
            count++;
        }

        std::cout << count << std::endl;
    }
    return 0;
}
