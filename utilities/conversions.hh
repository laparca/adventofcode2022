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

#include <string>
#include <id.hh>
#include <type_traits>

namespace laparca
{
	/**
	 * Utility template class that helps to convert std::string to other
	 * basic type.
	 *
	 * For automatically conversions to types it is more useful than
	 * create a function to for every basic type. You don't need to call
	 * std::stoi or std::stoul you just can use
	 * str_converter<int>::convert or str_converter<unsigned long>::convert.
	 */
	template<typename To>
	struct str_converter_t;

	template<typename To>
	str_converter_t<To> str_converter;
	
	#ifdef LAPARCA_STR_CONVERTER__
	#error LAPARCA_STR_CONVERTER__ was previously defined
	#endif

	#define LAPARCA_STR_CONVERTER__(type, func) \
	template<> struct str_converter_t<type> \
	{ \
		using is_algorithm = std::true_type; \
	    static type convert(const std::string& str) \
	    { \
	        return func(str); \
	    } \
        type operator()(const std::string& str) const \
        { \
           return convert(str); \
        } \
	}
	
	LAPARCA_STR_CONVERTER__(int, std::stoi);
	LAPARCA_STR_CONVERTER__(long, std::stol);
	LAPARCA_STR_CONVERTER__(long long, std::stoll);
	LAPARCA_STR_CONVERTER__(unsigned long, std::stoul);
	LAPARCA_STR_CONVERTER__(unsigned long long, std::stoull);
	LAPARCA_STR_CONVERTER__(float, std::stof);
	LAPARCA_STR_CONVERTER__(double, std::stod);
	LAPARCA_STR_CONVERTER__(long double, std::stold);
	LAPARCA_STR_CONVERTER__(std::string, id);
}

