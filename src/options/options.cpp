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
#include <string>
#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>


using namespace std::literals;


std::pair<options, std::pair<int, std::string>> options::parse(int argc, char ** argv) {
	options ret;

	try {
		existing_file_constraint input_file_constraint("input file");

		TCLAP::CmdLine command_line("gen-epub-book -- generate an ePub book from a simple plaintext descriptor", ' ', GEN_EPUB_BOOK_CPP_VERSION);
		TCLAP::UnlabeledValueArg<std::string> in_file("infile", "File to parse", true, "", &input_file_constraint, command_line);
		TCLAP::UnlabeledValueArg<std::string> out_file("outfile", "File to write the book to", true, "", "output file", command_line);

		command_line.parse(argc, argv);

		if(in_file.getValue().empty())
			return std::make_pair(ret, std::make_pair(1, "Input file can't be empty"s));
		else
			ret.in_file = in_file;

		if(out_file.getValue().empty())
			return std::make_pair(ret, std::make_pair(1, "Output file can't be empty"s));
		else
			ret.out_file = out_file;
	} catch(const TCLAP::ArgException & e) {
		return std::make_pair(ret, std::make_pair(1, std::string(argv[0]) + ": error: parsing arguments failed (" + e.error() + ") for argument " + e.argId()));
	}

	return std::make_pair(ret, std::make_pair(0, ""s));
}
