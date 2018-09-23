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


TEST_CASE("util::url_id()", "[util][url]") {
	REQUIRE(url_id("http://i.imgur.com/ViQ2WED.jpg") == "ViQ2WED_jpg");
	REQUIRE(url_id("https://rawcdn.githack.com/nabijaczleweli/nabijaczleweli.github.io/dev/src/writing_prompts/slim_shady.png") == "slim_shady_png");
	REQUIRE(url_id("https://img09.deviantart.net/e6c8/i/2015/138/8/0/the_pursuer_by_artsed-d7lbiua.jpg") == "the_pursuer_by_artsed-d7lbiua_jpg");
	REQUIRE(url_id("https://i.imgur.com/") == "");
	REQUIRE(url_id("https://i.imgur.com/.png") == "_png");
}

TEST_CASE("util::url_fname()", "[util][url]") {
	REQUIRE(url_fname("http://i.imgur.com/ViQ2WED.jpg") == "ViQ2WED.jpg");
	REQUIRE(url_fname("https://rawcdn.githack.com/nabijaczleweli/nabijaczleweli.github.io/dev/src/writing_prompts/slim_shady.png") == "slim_shady.png");
	REQUIRE(url_fname("https://img09.deviantart.net/e6c8/i/2015/138/8/0/the_pursuer_by_artsed-d7lbiua.jpg") == "the_pursuer_by_artsed-d7lbiua.jpg");
	REQUIRE(url_fname("https://i.imgur.com/") == "");
	REQUIRE(url_fname("https://i.imgur.com/.png") == ".png");
}
