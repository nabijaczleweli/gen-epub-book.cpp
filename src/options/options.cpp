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
#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>


using namespace std::literals;


std::tuple<options, int, std::string> options::parse(int argc, const char * const * argv) {
	options ret;

	try {
		existing_file_constraint input_file_constraint("input file");

		TCLAP::CmdLine command_line("gen-epub-book -- generate an ePub book from a simple plaintext descriptor", ' ', GEN_EPUB_BOOK_CPP_VERSION);
		TCLAP::UnlabeledValueArg<std::string> in_file("infile", "File to parse", true, "", &input_file_constraint, command_line);
		TCLAP::UnlabeledValueArg<std::string> out_file("outfile", "File to write the book to", true, "", "output file", command_line);

		command_line.setExceptionHandling(false);
		command_line.parse(argc, argv);

		if(in_file.getValue().empty())
			return std::make_tuple(ret, 1, "Input file can't be empty"s);
		else {
			ret.in_file = in_file;

			const auto slash_idx = ret.in_file.find_last_of("/\\");
			if(slash_idx == std::string::npos)
				ret.relative_root = "./";
			else
				ret.relative_root = ret.in_file.substr(0, slash_idx + 1);
		}

		if(out_file.getValue().empty())
			return std::make_tuple(ret, 1, "Output file can't be empty"s);
		else
			ret.out_file = out_file;
	} catch(const TCLAP::ArgException & e) {
		return std::make_tuple(ret, 1, std::string(argv[0]) + ": error: parsing arguments failed (" + e.error() + ") for " + e.argId());
	}

	return std::make_tuple(ret, 0, ""s);
}

bool operator==(const options & lhs, const options & rhs) {
	return lhs.in_file == rhs.in_file && lhs.out_file == rhs.out_file && lhs.relative_root == rhs.relative_root;
}
