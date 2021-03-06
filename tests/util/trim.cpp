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


#include "util.hpp"
#include <catch.hpp>


TEST_CASE("util::ltrim()", "[util][trim]") {
	std::string s = " thing ";
	REQUIRE(ltrim(s) == "thing ");

	s = " \tthing ";
	REQUIRE(ltrim(s) == "thing ");
}

TEST_CASE("util::rtrim()", "[util][trim]") {
	std::string s = " thing ";
	REQUIRE(rtrim(s) == " thing");

	s = " thing \t ";
	REQUIRE(rtrim(s) == " thing");
}

TEST_CASE("util::trim()", "[util][trim]") {
	std::string s = " thing ";
	REQUIRE(trim(s) == "thing");

	s = " \t thing \t ";
	REQUIRE(trim(s) == "thing");
}
