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


#pragma once


#include <nonstd/optional.hpp>
#include <ostream>
#include <string>
#include <utility>
#include <vector>


/// This class represents a single [`-I`nclude directory](https://nabijaczleweli.xyz/content/gen-epub-book/programmer.html#features-include-dirs).
///
/// It can be used to find and resolve files in that directory and get their IDs and packed paths.
///
/// See the `include_order` class for order-based resolution.
class include_dir {
public:
	nonstd::optional<std::string> name;
	std::string dir;


	/// Create an *unnamed* `-I`nclude directory which acts fully transparently.
	include_dir(std::string dir);

	/// Create a *named* `-I`nclude directory which prefixes its paths and IDs with its name.
	include_dir(std::string name, std::string dir);


	/// Check whether a file with the specified relative path is exists in this `-I`nclude directory.
	bool contains(const std::string & fname) const;

	/// Resolve a full path of the file with the specified relative path in this `-I`nclude directory.
	nonstd::optional<std::string> resolve(const std::string & fname) const;


	/// Get the (prefixed if *named*) ID of the specified file.
	std::string ebook_id(const std::string & fname) const;

	/// Get the (prefixed if *named*) packed path of the specified file.
	std::string ebook_filename(const std::string & fname) const;
};

/// This class represents an order-based list of
/// [`-I`nclude directories](https://nabijaczleweli.xyz/content/gen-epub-book/programmer.html#features-include-dirs).
///
/// It can be used to resolve files in an order, grabbing the first matching one.
class include_order {
public:
	std::vector<include_dir> order;


	/// Get the first directory in which a file with the specified relative path exists, or `nullptr`.
	include_dir * find(const std::string & fname);
	const include_dir * find(const std::string & fname) const;
};


std::ostream & operator<<(std::ostream & to, const include_dir & of);
std::ostream & operator<<(std::ostream & to, const include_order & of);

bool operator==(const include_dir & lhs, const include_dir & rhs);
bool operator==(const include_order & lhs, const include_order & rhs);
