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

std::vector<std::string> parse_input(std::istream& stream)
{
    std::vector<std::string> field;
    while(!stream.eof())
    {
        std::string line;
        std::getline(stream, line);

        if (line.empty()) break;

        field.emplace_back(std::move(line));
    }

    return field;
}

std::vector<std::vector<bool>> prepare_visibility(size_t width, size_t height)
{
    std::vector<std::vector<bool>> visibility(height);
    for (size_t row = 0; row < height; ++row)
    {
        visibility[row].resize(width);
        for (size_t column = 0; column < width; ++column)
        {
            visibility[row][column] = row == 0 || column == 0 || row == (height - 1) || column == (width - 1);
        }
    }

    return visibility;
}

size_t count_visible(const std::vector<std::vector<bool>>& visibility)
{
    return std::accumulate(std::begin(visibility), std::end(visibility), 0, [](int accum, const std::vector<bool>& v) {
        return accum + std::accumulate(std::begin(v), std::end(v), 0, [](int inner_accum, bool visible) {
            return inner_accum + (visible ? 1 : 0);
        });
    });
}

void calculate_external_visibility(const std::vector<std::string>& field, std::vector<std::vector<bool>>& visibility)
{
    size_t rows = field.size();
    size_t columns = field[0].size();
    auto up_down_visibility{visibility};
    auto down_up_visibility{visibility};
    auto left_right_visibility{visibility};
    auto right_left_visibility{visibility};


    for (size_t row = 1; row < rows - 1; ++row)
    {
        char left_right_max_height = field[row][0];
        char right_left_max_height = field[row][columns - 1];

        for (size_t column = 1; column < columns - 1; ++column)
        {
            left_right_visibility[row][column] = field[row][column] > left_right_max_height /*&& left_right_visibility[row][column - 1]*/;
            if (field[row][column] > left_right_max_height)
                left_right_max_height = field[row][column];

            right_left_visibility[row][columns - column - 1] = field[row][columns - column - 1] > right_left_max_height /*&& right_left_visibility[row][columns - column]*/;
            if (field[row][columns - column - 1] > right_left_max_height)
                right_left_max_height = field[row][columns - column - 1];
        }
    }
    for (size_t column = 1; column < columns - 1; ++column)
    {
        char up_down_max_height = field[0][column];
        char down_up_max_height = field[rows - 1][column];

        for (size_t row = 1; row < rows - 1; ++row)
        {
            up_down_visibility[row][column] = field[row][column] > up_down_max_height /*&& up_down_visibility[row - 1][column]*/;
            if (field[row][column] > up_down_max_height)
                up_down_max_height = field[row][column];

            down_up_visibility[rows - row - 1][column] = field[rows - row - 1][column] > down_up_max_height /*&& down_up_visibility[rows - row][column]*/;
            if (field[rows - row - 1][column] > down_up_max_height)
                down_up_max_height = field[rows - row - 1][column];
        }
    }
    for (size_t row = 1; row < rows - 1; ++row)
        for (size_t column = 1; column < columns - 1; ++column)
        {
            visibility[row][column] = up_down_visibility[row][column] || down_up_visibility[row][column] || left_right_visibility[row][column] || right_left_visibility[row][column];
        }
}

size_t calculate_max_scenic_score(const std::vector<std::string>& field)
{
    size_t rows = field.size();
    size_t columns = field[0].size();
    size_t max_scenic_score = 0;

    for (size_t row = 1; row < rows - 1; ++row)
        for (size_t column = 1; column < columns - 1; ++column)
        {
            size_t visibility_up_down = 0;
            for (size_t i = row+1; i < rows; i++)
            {
                visibility_up_down ++;
                if (field[row][column] <= field[i][column])
                    break;
            }
            size_t visibility_down_up = 0;
            for (int i = row-1; i >= 0; --i)
            {
                visibility_down_up ++;
                if (field[row][column] <= field[i][column])
                    break;
            }
            size_t visibility_left_right = 0;
            for (size_t i = column+1; i < columns; i++)
            {
                visibility_left_right ++;
                if (field[row][column] <= field[row][i])
                    break;
            }
            size_t visibility_right_left = 0;
            for (int i = column-1; i >= 0; --i)
            {
                visibility_right_left ++;
                if (field[row][column] <= field[row][i])
                    break;
            }

            size_t scenic_score = visibility_right_left * visibility_left_right * visibility_up_down * visibility_down_up;
            if (scenic_score > max_scenic_score)
                max_scenic_score = scenic_score;
        }
    return max_scenic_score;
}

int main()
{
    /* Part 1 */
    auto field = parse_input(std::cin);
    auto visibility = prepare_visibility(field.size(), field[0].size());

    calculate_external_visibility(field, visibility);

    std::cout << "There are " << count_visible(visibility) << " trees visibles" << std::endl;

    /* Part 2 */
    std::cout << "The maximun scenic score is " << calculate_max_scenic_score(field) << std::endl;
    return 0;
}
