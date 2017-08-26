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
#include <sys/stat.h>
#include <sys/types.h>


bool file_exists(const char * path) {
	auto f = std::fopen(path, "r");
	if(f)
		std::fclose(f);
	return f;
}

bool directory_exists(const char * path) {
	struct stat info;
	return stat(path, &info) == 0 && info.st_mode & S_IFDIR;
}

std::string path_id(std::string p) {
	auto path = path_fname(std::move(p));
	for(std::size_t i; (i = path.find(".")) != std::string::npos;)
		path.replace(i, 1, "_");
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
	auto path = url_fname(u);
	for(std::size_t i; (i = path.find(".")) != std::string::npos;)
		path.replace(i, 1, "_");
	return path;
}

std::string url_fname(const std::string & u) {
	return u.substr(u.find_last_of('/') + 1);
}

nonstd::optional<std::string> get_ebook_title(const std::string & in) {
	static const std::regex title_rgx("<!-- ePub title: \"([^\"]+)\" -->", std::regex_constants::ECMAScript | std::regex_constants::optimize);

	std::stringstream whole;
	whole << std::ifstream(in).rdbuf();

	std::cmatch title;
	if(std::regex_search(whole.str().c_str(), title, title_rgx))
		return {title[1]};
	else
		return {};
}

bool check_language(const char * lang) {
	// Stolen from http://stackoverflow.com/a/7036171/2851815
	static const std::regex lang_rgx("^(((en-GB-oed|i-ami|i-bnn|i-default|i-enochian|i-hak|i-klingon|i-lux|i-mingo|i-navajo|i-pwn|i-tao|i-tay|i-tsu|sgn-BE-FR|"
	                                 "sgn-BE-NL|sgn-CH-DE)|(art-lojban|cel-gaulish|no-bok|no-nyn|zh-guoyu|zh-hakka|zh-min|zh-min-nan|zh-xiang))|((([A-Za-z]{2,3}("
	                                 "-([A-Za-z]{3}(-[A-Za-z]{3}){0,2}))?)|[A-Za-z]{4}|[A-Za-z]{5,8})(-([A-Za-z]{4}))?(-([A-Za-z]{2}|[0-9]{3}))?(-([A-Za-z0-9]{5,"
	                                 "8}|[0-9][A-Za-z0-9]{3}))*(-([0-9A-WY-Za-wy-z](-[A-Za-z0-9]{2,8})+))*(-(x(-[A-Za-z0-9]{1,8})+))?)|(x(-[A-Za-z0-9]{1,8})+))$",
	                                 std::regex_constants::egrep | std::regex_constants::optimize);

	return std::regex_match(lang, lang_rgx);
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
