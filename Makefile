# The MIT License (MIT)

# Copyright (c) 2017 nabijaczleweli

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


include configMakefile


VERAR := $(foreach l,GEN_EPUB_BOOK_CPP,-D$(l)_VERSION='$($(l)_VERSION)')
SOURCES := $(sort $(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp src/**/**/**/*.cpp))
HEADERS := $(sort $(wildcard src/*.hpp src/**/*.hpp src/**/**/*.hpp src/**/**/**/*.hpp))

.PHONY : all clean exe


all : exe

clean :
	rm -rf $(OUTDIR)

exe : $(OUTDIR)gen-epub-book$(EXE)


$(OUTDIR)gen-epub-book$(EXE) : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES))) $(OS_OBJS)
	$(CXX) $(CXXAR) -o$@ $^ $(PIC) $(LNCXXAR)


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(VERAR) -c -o$@ $^