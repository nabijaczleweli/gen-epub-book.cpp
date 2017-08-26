// The MIT License (MIT)

// Copyright (c) 2017 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "date.hpp"
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>


static nonstd::optional<std::string> check_colonned_tz(const char (&tz)[6]);
static nonstd::optional<std::string> check_uncolonned_tz(const char (&tz)[5]);


nonstd::optional<std::pair<std::tm, std::string>> parse_datetime(const std::string & date_s, bool free_date) {
	auto dt = parse_rfc3339(date_s);

	if(free_date && !dt)
		dt = parse_rfc2822(date_s);

	return dt;
}

nonstd::optional<std::pair<std::tm, std::string>> parse_rfc3339(const std::string & date_s) {
	std::tm time{};
	char tz[6];

	std::istringstream ss(date_s);
	ss >> std::get_time(&time, "%Y-%m-%dT%H:%M:%S") >> tz;

	const auto tz_s = check_colonned_tz(tz);
	if(ss.fail() || date_s.size() != 25 || !tz_s)
		return {};
	else
		return nonstd::optional<std::pair<std::tm, std::string>>(nonstd::in_place, time, *tz_s);
}

nonstd::optional<std::pair<std::tm, std::string>> parse_rfc2822(const std::string & date_s) {
	std::tm time{};
	char tz[5];

	std::istringstream ss(date_s);
	ss >> std::get_time(&time, "%a, %d %b %Y %T ") >> tz;

	const auto tz_s = check_uncolonned_tz(tz);
	if(ss.fail() || date_s.size() != 31 || !tz_s)
		return {};
	else
		return nonstd::optional<std::pair<std::tm, std::string>>(nonstd::in_place, time, *tz_s);
}


static nonstd::optional<std::string> check_colonned_tz(const char (&tz)[6]) {
	if((tz[0] != '-' && tz[0] != '+') || !isdigit(tz[1]) || !isdigit(tz[2]) || (tz[3] != ':') || !isdigit(tz[4]) || !isdigit(tz[5]))
		return {};
	else
		return nonstd::optional<std::string>(nonstd::in_place, tz, 6);
}

static nonstd::optional<std::string> check_uncolonned_tz(const char (&tz)[5]) {
	if((tz[0] != '-' && tz[0] != '+') || !isdigit(tz[1]) || !isdigit(tz[2]) || !isdigit(tz[3]) || !isdigit(tz[4]))
		return {};
	else
		return {std::string(tz, 5).insert(3, 1, ':')};
}
