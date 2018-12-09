# [gen-epub-book](https://nabijaczleweli.xyz/content/gen-epub-book).cpp [![Build Status](https://travis-ci.org/nabijaczleweli/gen-epub-book.cpp.svg?branch=master)](https://travis-ci.org/nabijaczleweli/gen-epub-book.cpp) [![Licence](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
Generate an ePub book from a simple plaintext descriptor.

## [Manpage](https://rawcdn.githack.com/nabijaczleweli/gen-epub-book/man/gen-epub-book.cpp.1.html)

## Quickstart

Copy this somewhere:

```
Name: Simple ePub demonstration
Cover: cover.png

Image-Content: simple/chapter_image.png
Content: simple/ctnt.html

Author: nabijaczleweli
Date: 2017-02-08T15:30:18+01:00
Language: en-GB
```

Modify to your liking, then, assuming you put the file in "example/test.epupp" and want to write the result to "out/test.epub", run:

```sh
gen-epub-book example/test.epupp out/test.epub
```

For more detailed usage information and tag list, see the [manpage](https://rawcdn.githack.com/nabijaczleweli/gen-epub-book/man/gen-epub-book.rs.1.html),
and for a full guide see the [webpage](https://nabijaczleweli.xyz/content/gen-epub-book).

## Versions in other languages

The original in [AWK](https://github.com/nabijaczleweli/gen-epub-book).<br />
A rewrite in [Rust](https://github.com/nabijaczleweli/gen-epub-book.rs).<br />
A rewrite in [Scala](https://github.com/nabijaczleweli/gen-epub-book.scala).<br />
A rewrite in [JavaScript](https://github.com/nabijaczleweli/gen-epub-book.js).
