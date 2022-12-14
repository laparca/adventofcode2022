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
#include <iterators.hh>
#include <conversions.hh>
#include <algorithm.hh>
#include <io_util.hh>

int main()
{
    auto ecl = laparca::istream_to_container{std::cin}
                     | laparca::trans::convert_to<std::vector>() /* required due split. Split(T) return a std::vector<T>. In this case, istream_to_container cannot be used. */
                     | laparca::trans::split(std::string{""})
                     | laparca::trans::transform(
                           laparca::trans::transform(laparca::str_converter<int>) | laparca::trans::accum(0)
                       )
                     | laparca::trans::sort(laparca::functional::greater)
                     ;

    std::cout << "Max calories by one elf is " << *ecl.begin() << "; Max calories by top threee elfs is " << std::accumulate(ecl.begin(), ecl.begin() + 3, 0) << std::endl;
    
    return 0;
}

