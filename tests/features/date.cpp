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


#include "features/date.hpp"
#include <catch.hpp>


static const std::tm desired_datetime{18, 30, 15, 8, 1, 117, 3, 39, -1};
static const char * desired_timezone = "+01:00";

static void check_parsed_time(const std::pair<std::tm, std::string> & parsed);


TEST_CASE("features::parse_datetime(correct) -- rigid", "[features][date]") {
	const auto parsed = parse_datetime("2017-02-08T15:30:18+01:00", false);
	CHECK(parsed);
	check_parsed_time(*parsed);
}

TEST_CASE("features::parse_datetime(correct) -- free", "[features][date]") {
	{
		const auto parsed = parse_datetime("2017-02-08T15:30:18+01:00", true);
		CHECK(parsed);
		check_parsed_time(*parsed);
	}

	{
		const auto parsed = parse_datetime("Wed, 08 Feb 2017 15:30:18 +0100", true);
		CHECK(parsed);
		check_parsed_time(*parsed);
	}
}

TEST_CASE("features::parse_datetime(incorrect) -- rigid", "[features][date]") {
	CHECK_FALSE(parse_datetime("Wed, 08 Feb 2017 15:30:18 +0100", false));

	CHECK_FALSE(parse_datetime("2017-02-08T15:30:18+0100", false));
}

TEST_CASE("features::parse_datetime(incorrect) -- free", "[features][date]") {
	CHECK_FALSE(parse_datetime("2017-02-08T15:30:18+0100", true));
	CHECK_FALSE(parse_datetime("08 Feb 2017 15:30:18 +0100", true));
}

TEST_CASE("features::parse_rfc3339(correct)", "[features][date]") {
	const auto parsed = parse_rfc3339("2017-02-08T15:30:18+01:00");
	CHECK(parsed);
	check_parsed_time(*parsed);
}

TEST_CASE("features::parse_rfc3339(incorrect)", "[features][date]") {
	CHECK_FALSE(parse_rfc3339("2017-02-08T15:30:18+0100"));
	CHECK_FALSE(parse_rfc3339("2017-02-08 15:30:18+01:00"));
	CHECK_FALSE(parse_rfc3339("2017-02-08T15:30:18 +01:00"));

	CHECK_FALSE(parse_rfc3339("Wed, 08 Feb 2017 15:30:18 +0100"));
}

TEST_CASE("features::parse_rfc2822(correct)", "[features][date]") {
	const auto parsed = parse_rfc2822("Wed, 08 Feb 2017 15:30:18 +0100");
	CHECK(parsed);
	check_parsed_time(*parsed);
}

TEST_CASE("features::parse_rfc2822(incorrect)", "[features][date]") {
	CHECK_FALSE(parse_rfc2822("08 Feb 2017 15:30:18 +0100"));
	CHECK_FALSE(parse_rfc2822("Wed, 08 Feb 2017 15:30:18 +01:00"));
	CHECK_FALSE(parse_rfc2822("Wed, 08 Feb 2017 15:30:18:+0100"));

	CHECK_FALSE(parse_rfc2822("2017-02-08T15:30:18+0100"));
}


static void check_parsed_time(const std::pair<std::tm, std::string> & parsed) {
	REQUIRE(parsed.first.tm_sec == desired_datetime.tm_sec);
	REQUIRE(parsed.first.tm_min == desired_datetime.tm_min);
	REQUIRE(parsed.first.tm_hour == desired_datetime.tm_hour);
	REQUIRE(parsed.first.tm_mday == desired_datetime.tm_mday);
	REQUIRE(parsed.first.tm_mon == desired_datetime.tm_mon);
	REQUIRE(parsed.first.tm_year == desired_datetime.tm_year);

	if(parsed.first.tm_wday)
		REQUIRE(parsed.first.tm_wday == desired_datetime.tm_wday);
	if(parsed.first.tm_yday)
		REQUIRE(parsed.first.tm_yday == desired_datetime.tm_yday);
	if(parsed.first.tm_isdst)
		REQUIRE(parsed.first.tm_isdst == desired_datetime.tm_isdst);

	REQUIRE(parsed.second == desired_timezone);
}
