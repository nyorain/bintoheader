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

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
