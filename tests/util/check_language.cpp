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


// Examples from http://schneegans.de/lv
TEST_CASE("util::check_language() -- correct", "[util]") {
	REQUIRE(check_language("de"));
	REQUIRE(check_language("de-CH"));
	REQUIRE(check_language("de-DE-1901"));
	REQUIRE(check_language("es-419"));
	REQUIRE(check_language("sl-IT-nedis"));
	REQUIRE(check_language("en-US-boont"));
	REQUIRE(check_language("mn-Cyrl-MN"));
	REQUIRE(check_language("x-fr-CH"));
	REQUIRE(check_language("en-GB-boont-r-extended-sequence-x-private"));
	REQUIRE(check_language("sr-Cyrl"));
	REQUIRE(check_language("sr-Latn"));
	REQUIRE(check_language("hy-Latn-IT-arevela"));
	REQUIRE(check_language("zh-TW"));
}

TEST_CASE("util::check_language() -- incorrect", "[util]") {
	REQUIRE_FALSE(check_language("en_GB"));
	REQUIRE_FALSE(check_language("en*"));
}
