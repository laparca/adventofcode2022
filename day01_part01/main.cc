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

int elf_calories()
{
    std::string str;
    int calories_sum = 0;

    while(!std::cin.eof())
    {
        std::getline(std::cin, str);
        if (str == "")
            break;
        calories_sum += std::stoi(str);
    }

    return calories_sum;
}

int main()
{
    int max_calories = 0;
    int max_calories_elf = 0;
    int elf_count = 0;

    while(!std::cin.eof())
    {
        int calories = elf_calories();

        if (calories > max_calories)
        {
            max_calories = calories;
            max_calories_elf = elf_count;
        }

        elf_count ++;
    }

    std::cout << "Max Calories Elf = " << (max_calories_elf + 1) << " with " << max_calories << " calories" << std::endl;
    return 0;
}

