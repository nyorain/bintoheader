### Binary To Header File

This repository contains the code for a little program that is able to convert a (binary) file
into a valid C or C++ header file.

To use it, just build bintoheader.cpp with a C++11 compiler (Should work with GCC 5/6, Clang and
MSVC 2013/2015. Only tested on GCC 6). You can also simply use the provided makefile to
build (make), install([sudo] make install) or clean (make clean) it.

To configure compiler or install path just change the makefile, it is not that scary (13 lines
actually).

## Usage:

``````````````````````````````
bintoheader <file> [<file> ...] [-s {8|16|32|64}]

-s		Defines the size of the integers the data array in the header will consist of in bits.
		Can only be one of {8|16|32|64}. Command fails if the file size is not a multiple of the
		matching byte size (e.g. if -s32 is given the file size in bytes must be a multiple of 4).
		Defaulted to 8.
````````````````````````````

This will convert the file <file> into the valid header <file>.h.
The header can then be used with C++ (for C++11 and later it will define a constexpr array) or
with C99, in which case it will simply define a constant array.

This can be useful for including images, audio or shader files directly into the binary in a
platform independent way.

## License

All files in this repository as well as any resulting products (e.g. binaries or headers) are dual 
licensed. For more information see LICENSE.md. 
They can be used under the MIT License or under the following license:

	You are granted a perpetual, irrevocable license to copy, modify,
	publish, and distribute this file as you see fit.

This licensing option was chosing to make the software as easy as possible to use. There is no
attribution required.
The MIT License option is basically just for people that do not trust the custom license.
