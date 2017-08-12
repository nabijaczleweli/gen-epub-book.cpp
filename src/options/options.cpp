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


#include "options.hpp"
#include "existing_file_constraint.hpp"
#include "include_dir_constraint.hpp"
#include <iostream>
#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>


using namespace std::literals;


std::tuple<options, int, std::string> options::parse(int argc, const char * const * argv) {
	options ret;

	try {
		existing_file_constraint input_file_constraint("input file");
		include_dir_constraint incdirs_constraint;

		TCLAP::CmdLine command_line("gen-epub-book.cpp -- generate an ePub book from a simple plaintext descriptor\n"
		                            "https://nabijaczleweli.xyz/content/gen-epub-book",
		                            ' ', GEN_EPUB_BOOK_CPP_VERSION);
		TCLAP::UnlabeledValueArg<std::string> in_file("infile", "File to parse or '-' for stdin", true, "", &input_file_constraint, command_line);
		TCLAP::UnlabeledValueArg<std::string> out_file("outfile", "File to write the book to or '-' for stdout", true, "", "output file or '-'", command_line);
		TCLAP::MultiArg<std::string> incdirs("I", "include", "Add an include directory", false, &incdirs_constraint, command_line);

		command_line.setExceptionHandling(false);
		command_line.parse(argc, argv);

		if(in_file.getValue().empty())
			return std::make_tuple(ret, 1, "Input file can't be empty"s);
		else if(in_file.getValue() == "-") {
			ret.in_file = {};
			ret.include_dirs.order.emplace_back("./");
		} else {
			ret.in_file = {in_file};

			const auto slash_idx = in_file.getValue().find_last_of("/\\");
			if(slash_idx == std::string::npos)
				ret.include_dirs.order.emplace_back("./");
			else
				ret.include_dirs.order.emplace_back(in_file.getValue().substr(0, slash_idx + 1));
		}

		if(out_file.getValue().empty())
			return std::make_tuple(ret, 1, "Output file can't be empty"s);
		else if(out_file.getValue() == "-")
			ret.out_file = {};
		else
			ret.out_file = {out_file};

		for(auto && incdir : incdirs) {
			auto equal_idx = incdir.find('=');
			if(equal_idx == std::string::npos)
				ret.include_dirs.order.emplace_back(std::move(incdir));
			else
				ret.include_dirs.order.emplace_back(incdir.substr(0, equal_idx), incdir.c_str() + equal_idx + 1);
		}
	} catch(const TCLAP::ArgException & e) {
		auto arg_id = e.argId();
		if(arg_id == " ")
			arg_id = "undefined argument";
		return std::make_tuple(ret, 1, std::string(argv[0]) + ": error: parsing arguments failed (" + e.error() + ") for " + arg_id);
	} catch(const TCLAP::ExitException & e) {
		return std::make_tuple(ret, e.getExitStatus() ? e.getExitStatus() : 1, "");
	}

	return std::make_tuple(ret, 0, ""s);
}

bool operator==(const options & lhs, const options & rhs) {
	return lhs.in_file == rhs.in_file && lhs.out_file == rhs.out_file && lhs.include_dirs == rhs.include_dirs;
}

std::ostream & operator<<(std::ostream & to, const options & of) {
	to << "{ in=\"" << of.in_file.value_or("/dev/stdin") << "\", out=\"" << of.out_file.value_or("/dev/stdout") << "\", includes=" << of.include_dirs << " }";
	return to;
}
