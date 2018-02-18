// The MIT License (MIT)

// Copyright (c) 2017 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#pragma once


#include "features/include.hpp"
#include <ctime>
#include <istream>
#include <nonstd/optional.hpp>
#include <ostream>
#include <string>
#include <utility>
#include <uuid++.hh>
#include <vector>


namespace detail {
	struct book_parser;
}


enum class content_type { path, string, network };

struct content_data {
	std::string value;
	content_type type;
};

struct content_element {
	std::string id;
	std::string filename;
	content_data data;
};


class book {
private:
	friend struct detail::book_parser;
	friend bool operator==(const book & lhs, const book & rhs);

	uuid id;
	std::vector<content_element> content;
	std::vector<content_element> non_content;

	void write_content_table(void * epub);
	void write_table_of_contents(void * epub);
	void write_element(void * epub, const content_element & elem, std::vector<std::string> & written);
	void write_element_data(void * epub, const content_data & elem, bool wrap_strings = true);

public:
	std::string name;
	nonstd::optional<content_element> cover;
	std::string author;
	std::pair<std::tm, std::string> date;
	std::string language;
	nonstd::optional<content_data> description;


	template <class Iter>
	static book from(const include_order & incdirs, bool free_date, Iter lines_start, Iter lines_end);
	static book from(const include_order & incdirs, bool free_date, std::istream & descriptor);
	static book from(const include_order & incdirs, bool free_date, const std::string & descriptor);
	static book from(const include_order & incdirs, bool free_date, const char * descriptor);

	void write_to(const char * path);
};

bool operator==(const content_element & lhs, const content_element & rhs);
bool operator==(const content_data & lhs, const content_data & rhs);
bool operator==(const book & lhs, const book & rhs);


namespace detail {
	struct book_parser {
		unsigned int id;
		const include_order * incdirs;
		bool free_date;

		nonstd::optional<std::string> name;
		nonstd::optional<content_element> cover;
		nonstd::optional<std::string> author;
		nonstd::optional<std::pair<std::tm, std::string>> date;
		nonstd::optional<std::string> language;
		nonstd::optional<content_data> description;
		std::vector<content_element> content;
		std::vector<content_element> non_content;


		void take_line(const char * line);
		void take_line(const std::string & line);
		void take_line(const char * line, std::size_t len);
		void construct(book & b);
	};
}  // namespace detail


template <class Iter>
book book::from(const include_order & incdirs, bool free_date, Iter lines_start, Iter lines_end) {
	detail::book_parser p{};
	p.incdirs   = &incdirs;
	p.free_date = free_date;

	while(lines_start != lines_end)
		p.take_line(*lines_start++);

	book b{};
	p.construct(b);
	return b;
}
