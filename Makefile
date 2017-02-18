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


LDAR := $(PIC) $(foreach l,ext ext/cpr/lib,-L$(OUTDIR)$(l)) $(foreach l,cpr curl minizip z uuid++,-l$(l))
VERAR := $(foreach l,GEN_EPUB_BOOK_CPP CPR MINIZIP TCLAP UUID,-D$(l)_VERSION='$($(l)_VERSION)')
INCAR := $(foreach l,$(foreach l,$(foreach l,Catch cpr TCLAP minizip,$(l)/include) Optional,ext/$(l)) $(OUTDIR)ext/uuid/include,-isystem$(l))
BOOK_SOURCES := $(sort $(wildcard examples/*.epupp examples/**/*.epupp examples/**/**/*.epupp examples/**/**/**/*.epupp))
TEST_SOURCES := $(sort $(wildcard tests/*.cpp tests/**/*.cpp tests/**/**/*.cpp tests/**/**/**/*.cpp))
SOURCES := $(sort $(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp src/**/**/**/*.cpp))
ASSETS := $(sort $(wildcard assets/* assets/**/* assets/**/**/* assets/**/**/**/*))

.PHONY : all clean cpr minizip uuid exe tests run-tests examples

all : cpr minizip uuid exe tests run-tests examples

clean :
	rm -rf $(OUTDIR)

run-tests : $(OUTDIR)gen-epub-book-tests$(EXE)
	$^

exe : $(OUTDIR)gen-epub-book$(EXE)
examples : $(OUTDIR)gen-epub-book$(EXE) $(patsubst %.epupp,$(OUTDIR)%.epub,$(BOOK_SOURCES)) $(patsubst %.epupp,$(OUTDIR)%.mobi,$(BOOK_SOURCES))
tests : $(OUTDIR)gen-epub-book-tests$(EXE)
cpr : $(OUTDIR)ext/cpr/lib/libcpr.a
minizip : $(OUTDIR)ext/libminizip.a
uuid : $(OUTDIR)ext/libuuid++.a $(subst ext/uuid,$(OUTDIR)ext/uuid/include,$(sort $(wildcard ext/uuid/*.h ext/uuid/*.hh)))


$(OUTDIR)gen-epub-book$(EXE) : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES))) $(foreach f,$(ASSETS),$(BLDDIR)$(f)$(OBJ)) $(BLDDIR)mime_type/mime_type.o
	$(CXX) $(CXXAR) -o$@ $^ $(PIC) $(LDAR)

$(OUTDIR)gen-epub-book-tests$(EXE) : $(subst tests/,$(BLDDIR)test_obj/,$(subst .cpp,$(OBJ),$(TEST_SOURCES))) $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(filter-out src/main.cpp,$(SOURCES)))) $(foreach f,$(ASSETS),$(BLDDIR)$(f)$(OBJ)) $(BLDDIR)mime_type/mime_type.o
	$(CXX) $(CXXAR) -o$@ $^ $(PIC) $(LDAR)

$(OUTDIR)ext/cpr/lib/libcpr.a : ext/cpr/CMakeLists.txt
	@mkdir -p $(abspath $(dir $@)..)
	cd $(abspath $(dir $@)..) && CXXFLAGS="$(INCCXXAR) -DCURL_STATICLIB" $(CMAKE) -DUSE_SYSTEM_CURL=ON -DBUILD_CPR_TESTS=OFF $(abspath $(dir $^)) -GNinja
	cd $(abspath $(dir $@)..) && $(NINJA)

$(OUTDIR)ext/libminizip.a : $(subst ext/minizip/src,$(OUTDIR)ext/minizip,$(subst .c,$(OBJ),$(wildcard ext/minizip/src/*.c)))
	$(AR) crs $@ $^

$(OUTDIR)ext/libuuid++.a : ext/uuid/configure
	@mkdir -p $(OUTDIR)ext/uuid
	cd "$(OUTDIR)ext/uuid" && "$(realpath $^)" --with-cxx
	$(MAKE) -C $(OUTDIR)ext/uuid
	cp $(OUTDIR)ext/uuid/.libs/libuuid++.a $@

$(BLDDIR)mime_type/mime_type.o : $(BLDDIR)mime_type/mime_type.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) $(VERAR) -c -o$@ $^

$(BLDDIR)mime_type/mime_type.cpp : ext/mime.types
	@mkdir -p $(dir $@)
	$(ECHO) "#include <map>" > $@
	$(ECHO) "#include <string>" >> $@
	$(ECHO) "extern const std::map<std::string, const char *> mime_types;" >> $@
	$(ECHO) "const std::map<std::string, const char *> mime_types({" >> $@
	awk '/^[^#]/ {for(i = 2; i <= NF; ++i) print "\t{\"" $$i "\", \"" $$1 "\"},"}' $^ >> $@
	$(ECHO) "});" >> $@


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) $(VERAR) -c -o$@ $^

$(BLDDIR)test_obj/%$(OBJ) : tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(INCAR) -Isrc -c -o$@ $^

$(BLDDIR)assets/%$(OBJ) : assets/%
	@mkdir -p $(dir $@)
	$(ECHO) 'extern const char * const $(subst -,_,$(subst /,_,$(subst .,_,$^)));' > "$(BLDDIR)$^.cpp"
	$(ECHO) -n 'const char * const $(subst -,_,$(subst /,_,$(subst .,_,$^))) = R"DATA(' >> "$(BLDDIR)$^.cpp"
	cat $^ >> "$(BLDDIR)$^.cpp"
	$(ECHO) ')DATA";' >> "$(BLDDIR)$^.cpp"
	$(CXX) $(CXXAR) -c -o$@ "$(BLDDIR)$^.cpp"

$(OUTDIR)ext/minizip/%$(OBJ) : ext/minizip/src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCAR) -Iext/minizip/include/minizip -c -o$@ $^

$(OUTDIR)ext/uuid/include/% : ext/uuid/%
	@mkdir -p $(dir $@)
	cp $^ $@

$(OUTDIR)examples/%.mobi : $(OUTDIR)examples/%.epub
	@mkdir -p $(dir $@)
	$(CALIBRE_CONVERT) "$^" "$@"

$(OUTDIR)examples/%.epub : $(OUTDIR)gen-epub-book$(EXE) examples/%.epupp
	@mkdir -p $(dir $@)
	$< "$(filter-out $<,$^)" "$@"
