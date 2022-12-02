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
#include <sstream>

enum play {
    Rock,
    Paper,
    Scissors
};

enum result {
    Draw,
    Lost,
    Won
};

constexpr play oponent_to_play(char p)
{
    return static_cast<play>(p-'A');
}

constexpr play you_to_play(char p)
{
    return static_cast<play>(p-'X');
}

constexpr result calculate_result(play other, play you)
{
    constexpr result results[3][3] = {
        { Draw, Won, Lost },
        { Lost, Draw, Won },
        { Won, Lost, Draw }
    };

    return results[static_cast<int>(other)][static_cast<int>(you)];
}

constexpr int result_points(result r)
{
    constexpr int points[3] = { 3, 0, 6};
    return points[static_cast<int>(r)];
}

int main()
{
    int points = 0;
    while(!std::cin.eof()){
        std::string line;
        std::getline(std::cin, line);
        std::stringstream ss{line};
        char other, you;
        ss >> other >> you;

        play other_play = oponent_to_play(other);
        play you_play = you_to_play(you);

        int your_hand_points = static_cast<int>(you_play) + 1;
        result match_result = calculate_result(other_play, you_play);
        int match_points = result_points(match_result);

        points += your_hand_points + match_points;
    }

    std::cout << "You total score is " << points << std::endl;
    return 0;
}

