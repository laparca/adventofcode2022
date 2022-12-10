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
#include <io_util.hh>
#include <algorithm.hh>
int main()
{
    int cycle = 0;
    int accum = 1;
    std::vector<int> signal_strength;
    std::array<std::array<char, 40>, 6> canvas;

    auto increase_cycle = [&cycle, &accum, &signal_strength]() {
        constexpr static int checkpoints[6] = {20, 60, 100, 140, 180, 220};
        cycle++;
        if (std::find(std::begin(checkpoints), std::end(checkpoints), cycle) != std::end(checkpoints))
        {
            signal_strength.push_back(cycle * accum);
        }
    };

    auto draw = [&cycle, &accum, &canvas]() {
        int y = cycle / 40;
        int x = cycle % 40;
        if (x >= (accum - 1) && x < (accum-1+3))
            canvas[y][x] = '#';
        else
            canvas[y][x] = ' ';
    };

    laparca::istream_to_container{std::cin}
        | laparca::trans::transform(laparca::trans::split(' '))
        | laparca::trans::for_each([&increase_cycle, &draw, &accum](const std::vector<std::string>& pair)
          {
              draw();
              increase_cycle();
              if (pair[0] == "addx")
              {
                  draw();
                  increase_cycle();
                  accum += std::stoi(pair[1]);
              }
          })
        ;
    std::cout << "Signal sum is " << (signal_strength | laparca::trans::accum(0)) << std::endl;

    for(const auto& row : canvas)
    {
        for (const auto& pixel : row)
            std::cout << pixel;
        std::cout << std::endl;
    }
    return 0;
}