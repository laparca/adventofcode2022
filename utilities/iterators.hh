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

#include <conversions.hh>
#include <optional>
#include <functional>

namespace laparca
{
	/**
	 * This itereator read lines from an input stream and convert the readed
	 * value to the desired basic value. The end iterator represents the end
	 * of file or and empty line.
	 */
	template<typename T, typename Stream>
	class read_line_until_empty_iterator
	{
	public:
		using iterator_category = std::input_iterator_tag;
		using difference_type =  std::ptrdiff_t;
		using value_type = T;
		using pointer = const value_type*;
		using reference = const value_type&;

	public:
		read_line_until_empty_iterator() = default;
		read_line_until_empty_iterator(const read_line_until_empty_iterator&) = default;
		read_line_until_empty_iterator(Stream& stream)
		: stream_{&stream}
		, line_{get_line(stream_)}
		{}

		static std::optional<std::string> get_line(std::optional<Stream*> stream)
		{
			if (!stream.has_value() || stream.value()->eof()) return {};

			std::string line;
			std::getline(*stream.value(), line);

			return {line};
		}

		bool has_value() const
		{
			return stream_.has_value() && line_.has_value() && !line_.value().empty();
		}

		read_line_until_empty_iterator& operator=(const read_line_until_empty_iterator&) = default;
		bool operator==(const read_line_until_empty_iterator& it)
		{
			return !has_value() && !it.has_value();
		}

		bool operator!=(const read_line_until_empty_iterator& it)
		{
			return has_value() || it.has_value();
		}

		T operator*()
		{
			return str_converter<T>::convert(*line_);
		}

		read_line_until_empty_iterator& operator++()
		{
			line_ = get_line(stream_);
			return *this;
		}

		read_line_until_empty_iterator& operator++(int)
		{
			return this->operator++();
		}

	private:
		std::optional<Stream*> stream_;
		std::optional<std::string> line_;
	};
	template<typename T, typename Stream>
	read_line_until_empty_iterator(Stream&) -> read_line_until_empty_iterator<T, Stream>;

	/**
	 *
	 * This itereator read lines from an input stream. The end iterator
	 * represents the end of file.
	 */
	template<typename Stream>
	class read_line_iterator
	{
	public:
		using iterator_category = std::input_iterator_tag;
		using difference_type =  std::ptrdiff_t;
		using value_type = std::string;
		using pointer = const value_type*;
		using reference = const value_type&;

	public:
		read_line_iterator() = default;
		read_line_iterator(const read_line_iterator&) = default;
		read_line_iterator(Stream& stream)
		: stream_{&stream}
		, line_{get_line(stream_)}
		{}

		static std::optional<std::string> get_line(std::optional<Stream*> stream)
		{
			if (!stream.has_value() || stream.value()->eof()) return {};

			std::string line;
			std::getline(*stream.value(), line);

			return {line};
		}

		bool has_value() const
		{
			return stream_.has_value() && !stream_.value()->eof();
		}

		read_line_iterator& operator=(const read_line_iterator&) = default;
		bool operator==(const read_line_iterator& it)
		{
			return !has_value() && !it.has_value();
		}

		bool operator!=(const read_line_iterator& it)
		{
			return has_value() || it.has_value();
		}

		value_type operator*()
		{
			return *line_;
		}

		read_line_iterator& operator++()
		{
			line_ = get_line(stream_);
			return *this;
		}

		read_line_iterator& operator++(int)
		{
			return this->operator++();
		}

	private:
		std::optional<Stream*> stream_;
		std::optional<std::string> line_;
	};
	template<typename Stream>
	read_line_iterator(Stream&) -> read_line_iterator<Stream>;

	/**
	 * Iterator that calls to a function that returns and optional value.
	 * It reached the end when the function return an std::nullopt.
	 */
	template<typename T>
	class optional_generator_iterator
	{
	public:
		using iterator_category = std::input_iterator_tag;
		using difference_type =  std::ptrdiff_t;
		using value_type = int;
		using pointer = const int*;
		using reference = const int&;

		using generator_t = std::function<std::optional<T>(void)>;
	public:
		optional_generator_iterator() = default;
		optional_generator_iterator(const optional_generator_iterator&) = default;
		optional_generator_iterator(generator_t generator)
			: generator_{generator}
		, value_{generator()}
		{}

		optional_generator_iterator& operator=(const optional_generator_iterator&) = default;
		bool operator==(const optional_generator_iterator& it)
		{
			return !value_.has_value() && !it.value_.has_value();
		}

		bool operator!=(const optional_generator_iterator& it)
		{
			return value_.has_value() || it.value_.has_value();
		}

		T operator*()
		{
			return *value_;
		}

		optional_generator_iterator& operator++()
		{
			value_ = generator_();
			return *this;
		}

		optional_generator_iterator& operator++(int)
		{
			return this->operator++();
		}
	public:
		generator_t generator_;
		std::optional<T> value_;
	};
}
