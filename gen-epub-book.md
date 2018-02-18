gen-epub-book.cpp(1) -- Generate an ePub book from a simple plaintext descriptor
================================================================================

## SYNOPSIS

`gen-epub-book` IN_FILE OUT_FILE [OPTIONS]

## DESCRIPTION

Generate an ePub book from a simple plaintext descriptor.

## OPTIONS

  IN_FILE

    File to parse, must exist, must comply with the DESCRIPTOR FORMAT.

    Special case: '-' to write to stdin.

  OUT_FILE

    File to write the book to, parent directory must exist.

    Special case: '-' to write to stdout.

  -I [name=]path
  --include [name=]path

    Additional root directories, optionally named.

    If a file could not be found in the base directory, it will be looked for
    in the directories from this list, in order of specification.

    Details:
    https://nabijaczleweli.xyz/content/gen-epub-book/programmer.html#features-include-dirs

  -D --free-date

    Parse more datetime formats.

    See FREE DATE FORMAT FEATURE for details.

## DESCRIPTOR FORMAT

The descriptor consists of multiple lines in the format *"Key: Value"*, unknown
keys are ignored, lines that don't match the format are ignored.

  Name

    Required: yes
    Type: plaintext
    Value: e-book's title
    Amount: 1

  Content

    Required: no
    Type: file path
    Value: relative path to (X)HTML chunk
    Amount: any
    Remarks: see ADDITIONAL CONTENT PROCESSING

  String-Content

    Required: no
    Type: (X)HTML
    Value: (X)HTML string
    Amount: any

  Image-Content

    Required: no
    Type: file path
    Value: relative path to image to include in e-book
    Amount: any

  Network-Image-Content

    Required: no
    Type: file URL
    Value: URL of image to include in e-book
    Amount: any

  Cover

    Required: no
    Type: file path
    Value: relative path to image to use as e-book cover
    Amount: 0-1
    Remarks: exclusive with Network-Cover

  Network-Cover

    Required: no
    Type: file URL
    Value: URL to image to use as e-book cover
    Amount: 0-1
    Remarks: exclusive with Cover

  Include

    Required: no
    Type: file path
    Value: auxilliary file to include in e-book
    Amount: any

  Network-Include

    Required: no
    Type: file URL
    Value: URL of auxilliary file to include in e-book
    Amount: any

  Description

    Required: no
    Type: file URL
    Value: relative path to book description
    Amount: 0-1
    Remarks: exclusive with String-Description and Network-Description

  String-Description

    Required: no
    Type: (X)HTML
    Value: book description
    Amount: 0-1
    Remarks: exclusive with Description and Network-Description

  Network-Description

    Required: no
    Type: file URL
    Value: URL of book description
    Amount: 0-1
    Remarks: exclusive with Description and String-Description

  Author

    Required: yes
    Type: plaintext string
    Value: e-book's author
    Amount: 1

  Date

    Required: yes
    Type: RFC3339-compliant date
    Value: e-book's authoring/publishing date
    Amount: 1

  Language

    Required: yes
    Type: BCP47-compliant language code
    Value: language used in e-book
    Amount: 1

## ADDITIONAL CONTENT PROCESSING

When adding content using the `Content` entry, the file will additinally be
searched for a comment specifying the its name in the TOC in this format:

    <!-- ePub title: "TOC_NAME" -->

Where `TOC_NAME` is a string not containing the *"* character.

This will, on e-book readers, allow users to jump directly to the content
represented by the document containing this entry.

Optional.

## FREE DATE FORMAT FEATURE

With the -D/--free-date flag, you can enable the free date format feature:
&lt;<https://nabijaczleweli.xyz/content/gen-epub-book/programmer.html#features-free-date-format>&gt;.

The supported formats therewith are therefore:

  * RFC3339 (e.g. "2017-02-08T15:30:18+01:00"),
  * RFC2822 (e.g. "Wed, 08 Feb 2017 15:30:18 +0100").

## AUTHOR

Written by nabijaczleweli &lt;<nabijaczleweli@gmail.com>&gt;

## REPORTING BUGS

&lt;<https://github.com/nabijaczleweli/gen-epub-book.cpp/issues>&gt;

## SEE ALSO

&lt;<https://github.com/nabijaczleweli/gen-epub-book.cpp>&gt;<br />
&lt;<https://nabijaczleweli.xyz/content/gen-epub-book>&gt;
