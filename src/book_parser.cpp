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


#include "book.hpp"
#include "util.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>


using namespace std::literals;


template <class T>
static T try_option(nonstd::optional<T> opt, const char * name);


void detail::book_parser::take_line(const char * line) {
	take_line(std::string(line));
}

void detail::book_parser::take_line(const std::string & line) {
	auto colon_idx = line.find(':');
	if(colon_idx == std::string::npos)
		return;

	auto key   = line.substr(0, colon_idx);
	auto value = line.substr(colon_idx + 1);
	trim(key);
	trim(value);

	auto incdir = incdirs->find(value);

	if(key.empty() || value.empty())
		return;

	++id;

	if(key == "Name")
		if(name)
			throw "Name key specified more than once.";
		else
			name = value;
	else if(key == "Content")
		if(!incdir)
			throw "Content file \"" + value + "\" nonexistant.";
		else
			content.emplace_back(content_element{incdir->ebook_id(value), incdir->ebook_filename(value), *incdir->resolve(value), content_type::path});
	else if(key == "String-Content")
		content.emplace_back(content_element{"string-content-" + std::to_string(id), "string-data-" + std::to_string(id) + ".html", value, content_type::string});
	else if(key == "Image-Content")
		if(!incdir)
			throw "Image-Content file \"" + value + "\" nonexistant.";
		else {
			non_content.emplace_back(content_element{incdir->ebook_id(value), incdir->ebook_filename(value), *incdir->resolve(value), content_type::path});
			content.emplace_back(content_element{
			    "image-content-" + std::to_string(id), "image-data-" + std::to_string(id) + ".html",
			    "<center><img src=\""s + incdir->ebook_filename(value) + "\" alt=\"" + incdir->ebook_filename(value) + "\"></img></center>", content_type::string});
		}
	else if(key == "Network-Image-Content") {
		non_content.emplace_back(content_element{url_id(value), url_fname(value), value, content_type::network});
		content.emplace_back(content_element{"network-image-content-" + std::to_string(id), "network-image-data-" + std::to_string(id) + ".html",
		                                     "<center><img src=\"" + url_fname(value) + "\" alt=\"" + url_fname(value) + "\"></img></center>",
		                                     content_type::string});
	} else if(key == "Cover")
		if(cover)
			throw "[Network-]Cover key specified more than once.";
		else if(!incdir)
			throw "Cover file \"" + value + "\" nonexistant.";
		else {
			non_content.emplace_back(content_element{"cover-" + incdir->ebook_id(value), incdir->ebook_filename(value), *incdir->resolve(value), content_type::path});
			cover = content_element{"cover-data", "cover-data.html", "<center><img src=\""s + incdir->ebook_filename(value) + "\" alt=\"cover\"></img></center>",
			                        content_type::string};
		}
	else if(key == "Network-Cover")
		if(cover)
			throw "[Network-]Cover key specified more than once.";
		else {
			non_content.emplace_back(content_element{"network-cover-" + url_id(value), url_fname(value), value, content_type::network});
			cover = content_element{"network-cover-data", "network-cover-data.html", "<center><img src=\""s + url_fname(value) + "\" alt=\"cover\"></img></center>",
			                        content_type::string};
		}
	else if(key == "Include")
		if(!incdir)
			throw "Include file \"" + value + "\" nonexistant.";
		else
			non_content.emplace_back(content_element{incdir->ebook_id(value), incdir->ebook_filename(value), *incdir->resolve(value), content_type::path});
	else if(key == "Network-Include")
		non_content.emplace_back(content_element{url_id(value), url_fname(value), value, content_type::network});
	else if(key == "Author")
		if(author)
			throw "Author key specified more than once.";
		else
			author = value;
	else if(key == "Date") {
		if(date)
			throw "Date key specified more than once.";
		else {
			std::tm time{};
			char tz[6];

			std::istringstream ss(value);
			ss >> std::get_time(&time, "%Y-%m-%dT%H:%M:%S") >> tz;

			if(ss.fail() || value.size() != 25 ||
			   ((tz[0] != '-' && tz[0] != '+') || !isdigit(tz[1]) || !isdigit(tz[2]) || (tz[3] != ':') || !isdigit(tz[4]) || !isdigit(tz[5])))
				throw "Date malformed.";
			else
				date = std::make_pair(time, std::string(tz, 6));
		}
	} else if(key == "Language") {
		if(language)
			throw "Language key specified more than once.";
		else if(!check_language(value.c_str()))
			throw "Language " + value + " not valid BCP47.";
		else
			language = value;
	}
}

void detail::book_parser::take_line(const char * line, std::size_t len) {
	take_line(std::string(line, len));
}

void detail::book_parser::construct(book & b) {
	b.id.make(UUID_MAKE_V4);
	b.content     = std::move(content);
	b.non_content = std::move(non_content);
	b.name        = try_option(std::move(name), "Name");
	b.cover       = std::move(cover);
	b.author      = try_option(std::move(author), "Author");
	b.date        = try_option(std::move(date), "Date");
	b.language    = try_option(std::move(language), "Language");
}


template <class T>
static T try_option(nonstd::optional<T> opt, const char * name) {
	if(opt)
		return std::move(*opt);
	else
		throw "Required key "s + name + " not specified";
}
