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
#include "options/options.hpp"
#include <fstream>
#include <iostream>


#ifdef _WIN32
#define STDOUT "CON"
#else
#define STDOUT "/dev/stdout"
#endif


int main(int argc, char ** argv) {
	const auto opts_r = options::parse(argc, argv);
	if(std::get<1>(opts_r)) {
		std::cerr << std::get<2>(opts_r) << '\n';
		return std::get<1>(opts_r);
	}
	const auto opts = std::move(std::get<0>(opts_r));

	std::ifstream in(opts.in_file.value_or(""));
	try {
		book::from(opts.relative_root.c_str(), opts.in_file ? in : std::cin).write_to(opts.out_file.value_or(STDOUT).c_str());
	} catch(const char * s) {
		std::cerr << s << '\n';
		return 2;
	} catch(const std::string & s) {
		std::cerr << s << '\n';
		return 2;
	}
}
