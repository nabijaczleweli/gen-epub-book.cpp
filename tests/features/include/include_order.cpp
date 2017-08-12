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


#include "../../test_util.hpp"
#include "features/include.hpp"
#include <catch.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>


using namespace std::literals;


// Exact example from https://nabijaczleweli.xyz/content/gen-epub-book/programmer.html#features-include-dirs
TEST_CASE("features::include_order::find() const", "[features][include]") {
	const auto temp = temp_dir() + "/gen-epub-book.cpp/features/include/include_order/"s;

	include_dir book_dir(temp + "gep/special/");
	include_dir preview_dir("previews", temp + "previews/generated/out/");
	include_dir rendered_dir(temp + "rendered/output/");

	make_directory_recursive(book_dir.dir.c_str());
	make_directory_recursive(preview_dir.dir.c_str());
	make_directory_recursive(rendered_dir.dir.c_str());

	std::ofstream(temp + "gep/special/intro.html");
	std::ofstream(temp + "gep/special/book.epupp");

	std::ofstream(temp + "previews/generated/out/intro.html");
	std::ofstream(temp + "previews/generated/out/main.html");

	std::ofstream(temp + "rendered/output/intro.html");
	std::ofstream(temp + "rendered/output/main.html");
	std::ofstream(temp + "rendered/output/ending.html");

	include_order ord;
	ord.order.emplace_back(book_dir);
	ord.order.emplace_back(preview_dir);
	ord.order.emplace_back(rendered_dir);

	CHECK(ord.find("intro.html"));
	CHECK(ord.find("main.html"));
	CHECK(ord.find("ending.html"));

	REQUIRE(*ord.find("intro.html") == book_dir);
	REQUIRE(*ord.find("main.html") == preview_dir);
	REQUIRE(*ord.find("ending.html") == rendered_dir);

	CHECK(const_cast<const include_order &>(ord).find("intro.html"));
	CHECK(const_cast<const include_order &>(ord).find("main.html"));
	CHECK(const_cast<const include_order &>(ord).find("ending.html"));

	REQUIRE(*const_cast<const include_order &>(ord).find("intro.html") == book_dir);
	REQUIRE(*const_cast<const include_order &>(ord).find("main.html") == preview_dir);
	REQUIRE(*const_cast<const include_order &>(ord).find("ending.html") == rendered_dir);
}

TEST_CASE("features::include_order::operator<<()", "[features][include]") {
	include_order ord;
	std::ostringstream str;

	str << ord;
	REQUIRE(str.str() == "[]");
	str.str("");

	ord.order.emplace_back("examples\\");
	ord.order.emplace_back("examples");
	ord.order.emplace_back("ex", "examples");

	str << ord;
	REQUIRE(str.str() == "[\"examples\\\", \"examples/\", ex=\"examples/\"]");
	str.str("");
}

TEST_CASE("features::include_order::operator==()", "[features][include]") {
	include_order lhs;
	lhs.order.emplace_back("examples\\");
	lhs.order.emplace_back("examples");
	lhs.order.emplace_back("ex", "examples");

	include_order rhs;
	rhs.order = lhs.order;

	REQUIRE(lhs == rhs);
}
