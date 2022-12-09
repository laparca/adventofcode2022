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

#include <iterators.hh>
#include <iostream>

namespace laparca
{
    struct istream_to_container
    {
        using value_type = std::string;
        istream_to_container(std::istream& stream) : stream_{stream} {}
        istream_to_container(istream_to_container&& itc) : stream_{itc.stream_} {}

        inline read_line_iterator<std::istream> begin()
        {
            return {stream_};
        }

        inline read_line_iterator<std::istream> end()
        {
            return {};
        }

    private:
        std::istream& stream_;
    };
}