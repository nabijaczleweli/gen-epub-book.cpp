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


#include "book.hpp"
#include <cstring>


book book::from(const char * relroot, std::istream & descriptor) {
	detail::book_parser p{};
	p.relroot = relroot;
	for(std::string line; std::getline(descriptor, line);)
		p.take_line(line);

	book b{};
	p.construct(b);
	return b;
}

book book::from(const char * relroot, const std::string & descriptor) {
	return book::from(relroot, descriptor.c_str());
}

book book::from(const char * relroot, const char * descriptor) {
	detail::book_parser p{};
	p.relroot = relroot;
	for(const char *line_s = descriptor, *line_e = nullptr; (line_e = std::strchr(line_s, '\n'));) {
		p.take_line(line_s, line_e - line_s);
		line_s = line_e + 1;
	}

	book b{};
	p.construct(b);
	return b;
}

std::ostream & operator<<(std::ostream & out, const book & b) {
	out << '"' << b.name << "\" by " << b.author << '\n';
	return out;
}
