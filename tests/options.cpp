// The MIT License (MIT)

// Copyright (c) 2017 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#include "options/options.hpp"
#include "test_util.hpp"
#include <catch.hpp>
#include <fstream>
#include <string>
#include <tuple>


using namespace std::literals;


TEST_CASE("options::parse() -- not enough args", "[options]") {
	{
		const char * args[] = {"gen-epub-book.cpp tests"};
		REQUIRE(options::parse(1, args) == std::make_tuple(options{}, 1,
		                                                   "gen-epub-book.cpp tests: error: parsing arguments failed (Required arguments missing: infile, outfile) "
		                                                   "for undefined argument"s));
	}

	{
		const char * args[] = {"gen-epub-book.cpp tests", "LICENSE"};
		REQUIRE(options::parse(2, args) == std::make_tuple(options{}, 1,
		                                                   "gen-epub-book.cpp tests: error: parsing arguments failed (Required argument missing: outfile) "
		                                                   "for undefined argument"s));
	}
}

TEST_CASE("options::parse() -- incorrect args", "[options]") {
	const auto temp = temp_dir() + "/gen-epub-book.cpp/options/incorrect/"s;
	make_directory_recursive(temp.c_str());

	include_order temp_order;
	temp_order.order.emplace_back(temp);

	const auto nonexistant_file = temp + "nonexistant_file";
	const auto outfile          = temp + "outfile";
	const auto infile           = temp + "infile";
	std::ofstream{infile};

	{
		const char * args[] = {"gen-epub-book.cpp tests", nonexistant_file.c_str(), outfile.c_str()};
		REQUIRE(options::parse(3, args) == std::make_tuple(options{}, 1,
		                                                   "gen-epub-book.cpp tests: error: parsing arguments failed (Value '" + nonexistant_file +
		                                                       "' does not meet constraint: existing file or '-') for Argument: (--infile)"));
	}

	{
		const char * args[] = {"gen-epub-book.cpp tests", infile.c_str(), ""};
		REQUIRE(options::parse(3, args) == std::make_tuple(options{{infile}, {}, temp_order}, 1, "Output file can't be empty"s));
	}
}

TEST_CASE("options::parse() -- correct", "[options]") {
	const auto temp = temp_dir() + "/gen-epub-book.cpp/options/correct/"s;
	make_directory_recursive(temp.c_str());

	include_order temp_order;
	temp_order.order.emplace_back(temp);

	include_order dot_order;
	dot_order.order.emplace_back("./");

	const auto outfile = temp + "outfile";
	const auto infile  = temp + "infile";
	std::ofstream{infile};

	{
		const char * args[] = {"gen-epub-book.cpp tests", infile.c_str(), outfile.c_str()};
		REQUIRE(options::parse(3, args) == std::make_tuple(options{{infile}, {outfile}, temp_order}, 0, ""));
	}

	{
		const char * args[] = {"gen-epub-book.cpp tests", "LICENSE", outfile.c_str()};
		REQUIRE(options::parse(3, args) == std::make_tuple(options{{"LICENSE"}, {outfile}, dot_order}, 0, ""));
	}

	{
		const char * args[] = {"gen-epub-book.cpp tests", "-", outfile.c_str()};
		REQUIRE(options::parse(3, args) == std::make_tuple(options{{}, {outfile}, dot_order}, 0, ""));
	}

	{
		const char * args[] = {"gen-epub-book.cpp tests", infile.c_str(), "-"};
		REQUIRE(options::parse(3, args) == std::make_tuple(options{{infile}, {}, temp_order}, 0, ""));
	}

	{
		const char * args[] = {"gen-epub-book.cpp tests", "-", "-"};
		REQUIRE(options::parse(3, args) == std::make_tuple(options{{}, {}, dot_order}, 0, ""));
	}
}
