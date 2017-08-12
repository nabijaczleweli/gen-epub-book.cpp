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


TEST_CASE("util::directory_exists() -- nonexistant", "[util]") {
	const auto temp = temp_dir() + "/gen-epub-book.cpp/util/directory_exists/"s;
	make_directory_recursive(temp.c_str());

	REQUIRE_FALSE(directory_exists((temp + "nonexistant_dir").c_str()));
}

TEST_CASE("util::directory_exists() -- existant file", "[util]") {
	const auto temp = temp_dir() + "/gen-epub-book.cpp/util/directory_exists/"s;
	make_directory_recursive(temp.c_str());

	REQUIRE(directory_exists(temp.c_str()));
}
