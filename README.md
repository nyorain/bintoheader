### Binary To Header File

This repository contains the code for a tiny program that is able to convert a (binary) file
into a valid C or C++ header file holding an array with the files data.
This can be useful for including e.g. images, audio or shader files directly into the binary in a
platform independent way.

To use it, just build bintoheader.cpp with a C++11 compiler (Should work with GCC 5/6, Clang and
MSVC 2013/2015. Only tested on GCC 5/6). 
You can also simply use the provided makefile to build (make), install ([sudo] make install) or clean (make clean) it.
To configure compiler or install path just change the makefile, it is not that scary (13 lines
actually).
Note that getting it to compile with clang might require some extra include and link flags depending on the clang installation.

## Usage:

``````````````````````````````
  bintoheader [OPTION...]

  -i, --input arg   the input file. Can be any type of file, binary as well
                    as text
  -o, --output arg  the output file. By default the input flie name with a
                    '.h' suffix
  -s, --size arg    the byte size of the array {8|16|32|64}. Defaulted to 8.
                    Will fail if file size is not multiple.
  -n, --name arg    the name of the array in the c header. Defaulted to
                    'binaryData'
  -h, --help        Print this help string
````````````````````````````

This will convert the input file into the valid C header specified by the output argument that 
contains an array with the raw data of the input file.
The header can then be used with C++ (for C++11 and later it will define a constexpr array) or
with C99, in which case it will simply define a constant array.

## License

All files in this repository as well as any resulting products (e.g. binaries or headers) are dual 
licensed. For more information see LICENSE.md. 
They can be used under the MIT License or under the following license:

	You are granted a perpetual, irrevocable license to copy, modify,
	publish, and distribute this file as you see fit.

This licensing option was chosing to make the software as easy as possible to use. There is no
attribution required.
The MIT License option is basically just for people that do not trust the custom license.
