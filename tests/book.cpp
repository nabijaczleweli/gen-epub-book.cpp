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


#include "book.hpp"
#include <algorithm>
#include <catch.hpp>
#include <cstring>
#include <fstream>
#include <numeric>
#include <vector>


using namespace std::literals;


template <class T>
static std::vector<T> book_lines();
static std::vector<const char *> book_lines_without(const char * key);
static std::string book_str();
static void check_book(const book & b);


static const include_order examples_order{{{"examples/"s}, {"rel"s, "examples/relative_path_fuckery\\relative/path/"s}}};


TEST_CASE("book::from(C string iterator)", "[book]") {
	const auto lines = book_lines<const char *>();
	check_book(book::from(examples_order, lines.begin(), lines.end()));
}

TEST_CASE("book::from(std::string iterator)", "[book]") {
	const auto lines = book_lines<std::string>();
	check_book(book::from(examples_order, lines.begin(), lines.end()));
}

TEST_CASE("book::from(file stream)", "[book]") {
	std::ifstream in("examples/everything.epupp");
	check_book(book::from(examples_order, in));
}

TEST_CASE("book::from(std::string)", "[book]") {
	check_book(book::from(examples_order, book_str()));
}

TEST_CASE("book::from(C string)", "[book]") {
	check_book(book::from(examples_order, book_str().c_str()));
}

TEST_CASE("book::from() -- double", "[book]") {
	REQUIRE_THROWS_WITH(book::from(examples_order, "Name: l1\nName: l2\n"), "Name key specified more than once.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Author: l1\nAuthor: l2\n"), "Author key specified more than once.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Date: 2017-02-08T15:30:18+01:00\nDate: 2017-02-08T15:30:18+02:00\n"), "Date key specified more than once.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Language: en\nLanguage: pl\n"), "Language key specified more than once.");

	REQUIRE_THROWS_WITH(book::from(examples_order, "Cover: cover.png\nCover: cover.png\n"), "[Network-]Cover key specified more than once.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Network-Cover: l1\nCover: cover.png\n"), "[Network-]Cover key specified more than once.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Cover: cover.png\nNetwork-Cover: l2\n"), "[Network-]Cover key specified more than once.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Network-Cover: l1\nNetwork-Cover: l2\n"), "[Network-]Cover key specified more than once.");
}

TEST_CASE("book::from() -- incorrect", "[book]") {
	REQUIRE_THROWS_WITH(book::from(examples_order, "Content: simple/ctnt.htm\n"), "Content file \"simple/ctnt.htm\" nonexistant.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Image-Content: simple/chapter_image.jpg\n"), "Image-Content file \"simple/chapter_image.jpg\" nonexistant.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Cover: cover.jpg\n"), "Cover file \"cover.jpg\" nonexistant.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Date: 2017-02-08T15:30:18+01:0\n"), "Date malformed.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Date: 2017-02-08T15:30:18\n"), "Date malformed.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Date: 2017-02-08T1:30:18\n"), "Date malformed.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Date: Sun Feb 19 14:33:26 Central European Standard Time 2017\n"), "Date malformed.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Date: Sun, 19 Feb 2017 14:33:40 Central European Standard Time\n"), "Date malformed.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Date: yesterday\n"), "Date malformed.");
	REQUIRE_THROWS_WITH(book::from(examples_order, "Language: l1\n"), "Language l1 not valid BCP47.");
}

TEST_CASE("book::from() -- missing", "[book]") {
	for(auto key : {"Name", "Author", "Date", "Language"}) {
		const auto lines = book_lines_without(key);
		REQUIRE_THROWS_WITH(book::from(examples_order, lines.begin(), lines.end()), "Required key "s + key + " not specified");
	}
}

TEST_CASE("book::operator==()", "[book]") {
	REQUIRE(book::from(examples_order, book_str()) == book::from(examples_order, book_str()));
}


template <class T>
static std::vector<T> book_lines() {
	return {
	    "   Name: Everything we got, in one thing",                                                                                      //
	    "",                                                                                                                              //
	    "Content \t: simple/ctnt.html \t\t  ",                                                                                           //
	    "String-Content: <strong>SEIZE THE MEANS OF PRODUCTION!</strong>",                                                               //
	    "Image-Content: simple/chapter_image.png",                                                                                       //
	    "Image-Content: not_dead_yet.png",                                                                                               //
	    "Network-Image-Content: https://cdn.rawgit.com/nabijaczleweli/nabijaczleweli.github.io/dev/src/writing_prompts/slim_shady.png",  //
	    "",                                                                                                                              //
	    "Network-Cover: http://i.imgur.com/ViQ2WED.jpg",                                                                                 //
	    "",                                                                                                                              //
	    "Author: nabijaczleweli",                                                                                                        //
	    "Date: 2017-02-08T15:30:18+01:00",                                                                                               //
	    "Language: en-GB",                                                                                                               //
	};
}

static std::vector<const char *> book_lines_without(const char * key) {
	auto ln = book_lines<const char *>();
	ln.erase(std::remove_if(ln.begin(), ln.end(), [&](auto l) { return std::strstr(l, key); }));
	return ln;
}

static std::string book_str() {
	const auto data = book_lines<const char *>();
	return std::accumulate(data.begin(), data.end(), std::string(), [](auto && cur, auto line) { return cur + line + "\n"; });
}

static void check_book(const book & b) {
	REQUIRE(b.name == "Everything we got, in one thing");
	REQUIRE(b.cover == nonstd::make_optional(content_element{"network-cover-data", "network-cover-data.html",
	                                                         "<center><img src=\"ViQ2WED.jpg\" alt=\"cover\"></img></center>", content_type::string}));
	REQUIRE(b.author == "nabijaczleweli");
	REQUIRE(b.date.first.tm_sec == 18);
	REQUIRE(b.date.first.tm_min == 30);
	REQUIRE(b.date.first.tm_hour == 15);
	REQUIRE(b.date.first.tm_mday == 8);
	REQUIRE(b.date.first.tm_mon == 1);
	REQUIRE(b.date.first.tm_year == 117);
	REQUIRE(b.date.first.tm_isdst == 0);
	REQUIRE(b.language == "en-GB");
}
