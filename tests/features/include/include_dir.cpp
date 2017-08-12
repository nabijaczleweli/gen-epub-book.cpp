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


#include "features/include.hpp"
#include "util.hpp"
#include <catch.hpp>
#include <sstream>
#include <string>


using namespace std::literals;


TEST_CASE("features::include_dir(no name)", "[features][include]") {
	{
		include_dir dir("directory");
		CHECK_FALSE(dir.name.has_value());
		REQUIRE(dir.dir == "directory/");
	}

	{
		include_dir dir("directory/");
		CHECK_FALSE(dir.name.has_value());
		REQUIRE(dir.dir == "directory/");
	}

	{
		include_dir dir("directory\\");
		CHECK_FALSE(dir.name.has_value());
		REQUIRE(dir.dir == "directory\\");
	}
}

TEST_CASE("features::include_dir(name)", "[features][include]") {
	{
		include_dir dir("dir", "directory");
		REQUIRE(dir.name == nonstd::make_optional("dir"s));
		REQUIRE(dir.dir == "directory/");
	}

	{
		include_dir dir("dir", "directory/");
		REQUIRE(dir.name == nonstd::make_optional("dir"s));
		REQUIRE(dir.dir == "directory/");
	}

	{
		include_dir dir("dir", "directory\\");
		REQUIRE(dir.name == nonstd::make_optional("dir"s));
		REQUIRE(dir.dir == "directory\\");
	}
}

TEST_CASE("features::include_dir(no name)::contains() -- existing", "[features][include]") {
	{
		include_dir dir("examples");
		CHECK(dir.contains("cover.png"));
	}

	{
		include_dir dir("examples/");
		CHECK(dir.contains("cover.png"));
	}

	{
		include_dir dir("examples\\");
		CHECK(dir.contains("cover.png"));
	}
}

TEST_CASE("features::include_dir(name)::contains() -- existing", "[features][include]") {
	{
		include_dir dir("ex", "examples");
		CHECK(dir.contains("cover.png"));
	}

	{
		include_dir dir("ex", "examples/");
		CHECK(dir.contains("cover.png"));
	}

	{
		include_dir dir("ex", "examples\\");
		CHECK(dir.contains("cover.png"));
	}
}

TEST_CASE("features::include_dir(no name)::contains() -- nonexistant", "[features][include]") {
	{
		include_dir dir("out");
		CHECK_FALSE(dir.contains("cover.png"));
	}

	{
		include_dir dir("out/");
		CHECK_FALSE(dir.contains("cover.png"));
	}

	{
		include_dir dir("out\\");
		CHECK_FALSE(dir.contains("cover.png"));
	}
}

TEST_CASE("features::include_dir(name)::contains() -- nonexistant", "[features][include]") {
	{
		include_dir dir("o", "out");
		CHECK_FALSE(dir.contains("cover.png"));
	}

	{
		include_dir dir("o", "out/");
		CHECK_FALSE(dir.contains("cover.png"));
	}

	{
		include_dir dir("o", "out\\");
		CHECK_FALSE(dir.contains("cover.png"));
	}
}

TEST_CASE("features::include_dir(no name)::resolve() -- existing", "[features][include]") {
	{
		include_dir dir("examples");
		REQUIRE(dir.resolve("cover.png") == nonstd::make_optional("examples/cover.png"s));
	}

	{
		include_dir dir("examples/");
		REQUIRE(dir.resolve("cover.png") == nonstd::make_optional("examples/cover.png"s));
	}

	{
		include_dir dir("examples\\");
		REQUIRE(dir.resolve("cover.png") == nonstd::make_optional("examples\\cover.png"s));
	}
}

TEST_CASE("features::include_dir(name)::resolve() -- existing", "[features][include]") {
	{
		include_dir dir("ex", "examples");
		REQUIRE(dir.resolve("cover.png") == nonstd::make_optional("examples/cover.png"s));
	}

	{
		include_dir dir("ex", "examples/");
		REQUIRE(dir.resolve("cover.png") == nonstd::make_optional("examples/cover.png"s));
	}

	{
		include_dir dir("ex", "examples\\");
		REQUIRE(dir.resolve("cover.png") == nonstd::make_optional("examples\\cover.png"s));
	}
}

TEST_CASE("features::include_dir(no name)::resolve() -- nonexistant", "[features][include]") {
	{
		include_dir dir("out");
		CHECK_FALSE(dir.resolve("cover.png"));
	}

	{
		include_dir dir("out/");
		CHECK_FALSE(dir.resolve("cover.png"));
	}

	{
		include_dir dir("out\\");
		CHECK_FALSE(dir.resolve("cover.png"));
	}
}

TEST_CASE("features::include_dir(name)::resolve() -- nonexistant", "[features][include]") {
	{
		include_dir dir("o", "out");
		CHECK_FALSE(dir.resolve("cover.png"));
	}

	{
		include_dir dir("o", "out/");
		CHECK_FALSE(dir.resolve("cover.png"));
	}

	{
		include_dir dir("o", "out\\");
		CHECK_FALSE(dir.resolve("cover.png"));
	}
}

TEST_CASE("features::include_dir(no name)::ebook_id()", "[features][include]") {
	{
		include_dir dir("examples");
		REQUIRE(dir.ebook_id("cover.png") == path_id("cover.png"));
	}

	{
		include_dir dir("examples/");
		REQUIRE(dir.ebook_id("cover.png") == path_id("cover.png"));
	}

	{
		include_dir dir("examples\\");
		REQUIRE(dir.ebook_id("cover.png") == path_id("cover.png"));
	}
}

TEST_CASE("features::include_dir(name)::ebook_id()", "[features][include]") {
	{
		include_dir dir("ex", "examples");
		REQUIRE(dir.ebook_id("cover.png") == "ex--" + path_id("cover.png"));
	}

	{
		include_dir dir("ex", "examples/");
		REQUIRE(dir.ebook_id("cover.png") == "ex--" + path_id("cover.png"));
	}

	{
		include_dir dir("ex", "examples\\");
		REQUIRE(dir.ebook_id("cover.png") == "ex--" + path_id("cover.png"));
	}
}

TEST_CASE("features::include_dir(no name)::ebook_filename()", "[features][include]") {
	{
		include_dir dir("examples");
		REQUIRE(dir.ebook_filename("cover.png") == path_fname("cover.png"));
	}

	{
		include_dir dir("examples/");
		REQUIRE(dir.ebook_filename("cover.png") == path_fname("cover.png"));
	}

	{
		include_dir dir("examples\\");
		REQUIRE(dir.ebook_filename("cover.png") == path_fname("cover.png"));
	}
}

TEST_CASE("features::include_dir(name)::ebook_filename()", "[features][include]") {
	{
		include_dir dir("ex", "examples");
		REQUIRE(dir.ebook_filename("cover.png") == "ex/" + path_fname("cover.png"));
	}

	{
		include_dir dir("ex", "examples/");
		REQUIRE(dir.ebook_filename("cover.png") == "ex/" + path_fname("cover.png"));
	}

	{
		include_dir dir("ex", "examples\\");
		REQUIRE(dir.ebook_filename("cover.png") == "ex/" + path_fname("cover.png"));
	}
}

TEST_CASE("features::include_dir::operator<<()", "[features][include]") {
	{
		std::ostringstream str;
		str << include_dir("directory");
		REQUIRE(str.str() == "\"directory/\"");
	}

	{
		std::ostringstream str;
		str << include_dir("directory\\");
		REQUIRE(str.str() == "\"directory\\\"");
	}

	{
		std::ostringstream str;
		str << include_dir("dir", "directory");
		REQUIRE(str.str() == "dir=\"directory/\"");
	}
}

TEST_CASE("features::include_dir::operator==()", "[features][include]") {
	{
		include_dir lhs("directory");
		include_dir rhs("directory/");
		REQUIRE(lhs == rhs);
	}

	{
		include_dir lhs("dir", "directory");
		include_dir rhs("dir", "directory/");
		REQUIRE(lhs == rhs);
	}
}
