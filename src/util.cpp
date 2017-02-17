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
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <regex>
#include <sstream>


bool file_exists(const char * path) {
	auto f = std::fopen(path, "r");
	if(f)
		std::fclose(f);
	return f;
}

std::string path_id(std::string p) {
	auto path = path_fname(std::move(p));
	for(std::size_t i; (i = path.find(".")) != std::string::npos;)
		path.replace(i, 1, "-");
	return path;
}

std::string path_fname(std::string p) {
	for(std::size_t i; (i = p.find("\\")) != std::string::npos;)
		p.replace(i, 1, "/");
	for(std::size_t i; (i = p.find("../")) != std::string::npos;)
		p.replace(i, 3, "");
	for(std::size_t i; (i = p.find("./")) != std::string::npos;)
		p.replace(i, 2, "");
	for(std::size_t i; (i = p.find("/")) != std::string::npos;)
		p.replace(i, 1, "-");
	return p;
}

std::string url_id(const std::string & u) {
	const auto post_slash_idx = u.find_last_of('/') + 1;
	return u.substr(post_slash_idx, u.find_last_of('.') - post_slash_idx);
}

std::string url_fname(const std::string & u) {
	return u.substr(u.find_last_of('/') + 1);
}

std::experimental::optional<std::string> get_ebook_title(const std::string & in) {
	static const std::regex title_rgx("<!-- ePub title: \"([^\"]+)\" -->", std::regex_constants::ECMAScript | std::regex_constants::optimize);

	std::stringstream whole;
	whole << std::ifstream(in).rdbuf();

	std::cmatch title;
	if(std::regex_search(whole.str().c_str(), title, title_rgx))
		return {title[1]};
	else
		return {};
}

std::string & ltrim(std::string & s) {
	s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), [](char c) { return std::isspace(c); }));
	return s;
}

std::string & rtrim(std::string & s) {
	s.erase(std::find_if_not(s.rbegin(), s.rend(), [](char c) { return std::isspace(c); }).base(), s.end());
	return s;
}

std::string & trim(std::string & s) {
	return ltrim(rtrim(s));
}
