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


#include <ctime>
#include <istream>
#include <optional.hpp>
#include <ostream>
#include <string>
#include <utility>
#include <uuid++.hh>
#include <vector>


namespace detail {
	struct book_parser;
}

enum class content_type { path, string, network };

struct content_element {
	std::string id;
	std::string filename;
	std::string data;
	content_type tp;
};

class book {
private:
	friend struct detail::book_parser;

	uuid id;
	std::vector<content_element> content;
	std::vector<content_element> non_content;

	void write_content_table(void * epub);
	void write_table_of_contents(void * epub);
	void write_element(void * epub, const content_element & elem);

public:
	std::string name;
	std::experimental::optional<content_element> cover;
	std::string author;
	std::pair<std::tm, std::string> date;
	std::string language;


	template <class Iter>
	static book from(const char * relroot, Iter lines_start, Iter lines_end);
	static book from(const char * relroot, std::istream & descriptor);
	static book from(const char * relroot, const std::string & descriptor);
	static book from(const char * relroot, const char * descriptor);

	void write_to(const char * path);
};

namespace detail {
	struct book_parser {
		unsigned int id;
		const char * relroot;
		std::experimental::optional<std::string> name;
		std::experimental::optional<content_element> cover;
		std::experimental::optional<std::string> author;
		std::experimental::optional<std::pair<std::tm, std::string>> date;
		std::experimental::optional<std::string> language;
		std::vector<content_element> content;
		std::vector<content_element> non_content;

		void take_line(const char * line);
		void take_line(const std::string & line);
		void take_line(const char * line, std::size_t len);
		void construct(book & b);
	};
}


template <class Iter>
book book::from(const char * relroot, Iter lines_start, Iter lines_end) {
	detail::book_parser p{};
	p.relroot = relroot;
	for(auto itr = lines_start; itr != lines_end; ++lines_start)
		p.take_line(*itr);

	book b{};
	p.construct(b);
	return b;
}
