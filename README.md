### Binary To Header File

This repository contains the code for a tiny program that is able to convert a (binary) file
into a valid C or C++ header file holding an array with the file's data.
This can be useful for including e.g. images, audio or compiled shader files directly into the
binary in a platform independent way.

To use it, just build bintoheader.cpp with a C++1z compiler. Currently only working using g++-7.
You can also simply use the provided makefile to build (make), install ([sudo] make install) or clean (make clean) it.
To configure compiler or install path just change the makefile, it is not that scary.
Alternatively use the provided meson project (e.g. as subproject).

## Usage:

```
Transforms a file into a c/cpp header holding its binary data as array.
Valid Usages:

	bintoheader <INPUT> [<OUTPUT>] [<ARGS>]
	bintoheader [<ARGS>]

The following arguments are recognized:

	-i	The input file
	-o	The output file [defaulted to '<input>.h']
	-name	The name of the c array
		Defaulted to '<input>.h', with all '.' replaced by '.'
	-size	The size of the array values. Defaulted to 8.
		The file size must be a multiple of the size value
	-help	Show this error message

The program is released into the public domain
A copy of the source can be obtained under
https://github.com/nyorain/bintoheader
```

The header can then be used with C++ (for C++11 and later it will define a constexpr array) or
with C99, in which case it will simply define a constant array.
