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


TEST_CASE("util::path_id()", "[util][path]") {
	REQUIRE(path_id("simple/chapter_image.png") == "simple-chapter_image_png");
	REQUIRE(path_id("simple/ctnt.html") == "simple-ctnt_html");
	REQUIRE(path_id("../cover.png") == "cover_png");
	REQUIRE(path_id("relative/path/../green_ass_dog.html") == "relative-path-green_ass_dog_html");
	REQUIRE(path_id("./../relative_path_fuckery\\relative/../relative/path\\../../relative/path/dead_santa.html") ==
	        "relative_path_fuckery-relative-relative-path-relative-path-dead_santa_html");
	REQUIRE(path_id("../cover") == "cover");
}

TEST_CASE("util::path_fname()", "[util][path]") {
	REQUIRE(path_fname("simple/chapter_image.png") == "simple-chapter_image.png");
	REQUIRE(path_fname("simple/ctnt.html") == "simple-ctnt.html");
	REQUIRE(path_fname("../cover.png") == "cover.png");
	REQUIRE(path_fname("relative/path/../green_ass_dog.html") == "relative-path-green_ass_dog.html");
	REQUIRE(path_fname("./../relative_path_fuckery\\relative/../relative/path\\../../relative/path/dead_santa.html") ==
	        "relative_path_fuckery-relative-relative-path-relative-path-dead_santa.html");
	REQUIRE(path_fname("../cover") == "cover");
}
