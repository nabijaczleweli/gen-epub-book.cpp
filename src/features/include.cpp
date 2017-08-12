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


#include "include.hpp"
#include "../util.hpp"
#include <algorithm>


include_dir::include_dir(std::string new_dir) : dir(std::move(new_dir)) {
	if(dir.back() != '/' && dir.back() != '\\')
		dir.push_back('/');
}

include_dir::include_dir(std::string new_name, std::string new_dir) : include_dir(std::move(new_dir)) {
	name = std::move(new_name);
}

nonstd::optional<std::string> include_dir::resolve(const std::string & fname) const {
	const auto path = dir + fname;
	if(file_exists(path.c_str()))
		return {path};
	else
		return {};
}

bool include_dir::contains(const std::string & fname) const {
	return file_exists((dir + fname).c_str());
}

std::string include_dir::ebook_id(const std::string & fname) const {
	auto id = path_id(fname);
	if(name) {
		id.insert(0, "--");
		id.insert(0, *name);
	}
	return id;
}

std::string include_dir::ebook_filename(const std::string & fname) const {
	auto fn = path_fname(fname);
	if(name) {
		fn.insert(0, 1, '/');
		fn.insert(0, *name);
	}
	return fn;
}


include_dir * include_order::find(const std::string & fname) {
	const auto ret = std::find_if(order.begin(), order.end(), [&](auto && o) { return o.contains(fname); });
	if(ret == order.end())
		return nullptr;
	else
		return &*ret;
}

const include_dir * include_order::find(const std::string & fname) const {
	const auto ret = std::find_if(order.begin(), order.end(), [&](auto && o) { return o.contains(fname); });
	if(ret == order.end())
		return nullptr;
	else
		return &*ret;
}


std::ostream & operator<<(std::ostream & to, const include_dir & of) {
	if(of.name)
		to << *of.name << '=';
	to << '"' << of.dir << '"';
	return to;
}

std::ostream & operator<<(std::ostream & to, const include_order & of) {
	bool first = true;

	to << '[';
	for(auto && o : of.order) {
		if(!first)
			to << ", ";
		to << o;
		first = false;
	}
	to << ']';

	return to;
}


bool operator==(const include_dir & lhs, const include_dir & rhs) {
	return lhs.name == rhs.name && lhs.dir == rhs.dir;
}

bool operator==(const include_order & lhs, const include_order & rhs) {
	return lhs.order == rhs.order;
}
