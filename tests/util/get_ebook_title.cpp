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


#include "../test_util.hpp"
#include "util.hpp"
#include <catch.hpp>
#include <fstream>
#include <string>


using namespace std::literals;


TEST_CASE("util::get_ebook_title() -- nonexistant", "[util]") {
	const auto temp = temp_dir() + "/gen-epub-book.cpp/util/get_ebook_title/"s;
	make_directory_recursive(temp.c_str());

	REQUIRE(get_ebook_title(temp + "nonexistant_file") == std::experimental::nullopt);
}

TEST_CASE("util::get_ebook_title() -- no title", "[util]") {
	REQUIRE(get_ebook_title("src/book_parser.cpp") == std::experimental::nullopt);
	REQUIRE(get_ebook_title("examples/everything.epupp") == std::experimental::nullopt);
	REQUIRE(get_ebook_title("examples/relative_path_fuckery/relative/path/dead_santa.html") == std::experimental::nullopt);
}

TEST_CASE("util::get_ebook_title() -- close", "[util]") {
	const auto temp = temp_dir() + "/gen-epub-book.cpp/util/get_ebook_title/"s;
	make_directory_recursive(temp.c_str());

	std::ofstream(temp + "quote") << R"(<!-- ePub title: """ -->)";
	std::ofstream(temp + "no_right_quote") << R"(<!-- ePub title: "No right quote -->)";
	std::ofstream(temp + "no_left_quote") << R"(<!-- ePub title: No right quote" -->)";
	std::ofstream(temp + "unclosed_comment") << R"(<!-- ePub title: "No right quote")";
	std::ofstream(temp + "unopened_comment") << R"(ePub title: "No right quote" -->)";

	REQUIRE(get_ebook_title(temp + "quote") == std::experimental::nullopt);
	REQUIRE(get_ebook_title(temp + "no_right_quote") == std::experimental::nullopt);
	REQUIRE(get_ebook_title(temp + "no_left_quote") == std::experimental::nullopt);
	REQUIRE(get_ebook_title(temp + "unclosed_comment") == std::experimental::nullopt);
	REQUIRE(get_ebook_title(temp + "unopened_comment") == std::experimental::nullopt);
}

TEST_CASE("util::get_ebook_title() -- with title", "[util]") {
	const auto temp = temp_dir() + "/gen-epub-book.cpp/util/get_ebook_title/"s;
	make_directory_recursive(temp.c_str());

	std::ofstream(temp + "in_cplusplus.cpp") << R"(
		// <!-- ePub title: "In C++" -->
		std::string url_fname(const std::string & u) {
			return u.substr(u.find_last_of('/') + 1);
		}

		// <!-- ePub title: "In C++, second" -->
	)";

	REQUIRE(get_ebook_title("examples/simple/ctnt.html") == std::experimental::make_optional("Chapter 1, Where everything's still going mostly right"s));
	REQUIRE(get_ebook_title(temp + "in_cplusplus.cpp") == std::experimental::make_optional("In C++"s));
}
