// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org>

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cstring>

#include "flags.hpp"

// file template to output
constexpr auto fileTemplate = 1 + R"STR(
#ifndef BINARY_%i_H_INC
#define BINARY_%i_H_INC

#include <stdint.h>

#if defined(__cplusplus) && __cplusplus >= 201103L
	constexpr
#else
	const
#endif

%t %n[] = {
	%d
};

#endif // header guard)STR";

void replace(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty()) return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void stripPath(std::string& str)
{
	auto last = str.find_last_of("/");
	if(last != str.npos)
		str.erase(0, last + 1);
}

//how many chars should be in one line (excluding beginning tab).
constexpr auto lineLength = 80;

std::vector<std::uint8_t> loadFile(const std::string& file)
{
	auto openmode = std::ios::ate | std::ios::binary;

	std::ifstream ifs(file, openmode);
	if(!ifs.is_open()) {
		std::cerr << "loadFile: couldnt open input file " << file << "\n";
		std::cerr << "error: " << std::strerror(errno) << "\n";
		exit(EXIT_FAILURE);
	}

	auto size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::vector<std::uint8_t> buffer(size);
	auto data = reinterpret_cast<char*>(buffer.data());
	if(!ifs.read(data, size)) {
		std::cerr << "loadFile: failed to read input file " << file << "\n";
		std::cerr << "error: " << std::strerror(errno) << "\n";
		exit(EXIT_FAILURE);
	}

	return buffer;
}

void outputData(const std::vector<std::uint8_t>& data, const std::string& file,
	const std::string& name, int size)
{
	auto fname = file;
	std::ofstream ofs(fname);
	if(!ofs.is_open()) {
		std::cerr << "outputData: couldnt open output file " << file << "\n";
		std::cerr << "error: " << std::strerror(errno) << "\n";
		exit(EXIT_FAILURE);
	}

	auto byteSize = size / 8;
	if(data.size() % byteSize) {
		std::cerr << "outputData: file size is not a multiple of specified integer size\n";
		std::cerr << "\tfile size: " << data.size() << " bytes\n";
		std::cerr << "\tspecified size: " << size / 8 << " bytes\n";
		exit(EXIT_FAILURE);
	}

	std::string dataString;
	dataString.reserve(data.size() * 5); //approx.

	auto currColSize = 0u;
	for(auto i = 0u; i < data.size() / byteSize; ++i) {
		std::uint64_t value = 0;
		auto ptr = &data[i * byteSize];

		if(size == 8) value = *reinterpret_cast<const std::uint8_t*>(ptr);
		else if(size == 16) value = *reinterpret_cast<const std::uint16_t*>(ptr);
		else if(size == 32) value = *reinterpret_cast<const std::uint32_t*>(ptr);
		else if(size == 64) value = *reinterpret_cast<const std::uint64_t*>(ptr);

		auto str = std::to_string(value);
		if(i < data.size() / byteSize - 1) str += ", "; //check if last value or not
		if(currColSize + str.size() > lineLength)
		{
			dataString.append("\n\t");
			currColSize = 0;
		}
		currColSize += str.size();
		dataString.append(str);
	}

	std::string outputTypeName = "uint" + std::to_string(size) + "_t";
	std::string includeGuard = name;
	for(auto& ch : includeGuard) ch = std::toupper(ch);

	std::string txt = fileTemplate;
	replace(txt, "%t", outputTypeName);
	replace(txt, "%d", dataString);
	replace(txt, "%n", name);
	replace(txt, "%i", includeGuard);

	ofs << txt;
}

int main(int argc, char** argv)
{
	// parse args
	const flags::args args(argc, argv);

	// help
	auto help = args.get<bool>("help", false);
	if(help) {
		std::cout << "Transforms a file into a c/cpp header holding its binary data as array.\n";
		std::cout << "Valid Usages:\n\n";
		std::cout << "\tbintoheader <INPUT> [<OUTPUT>] [<ARGS>]\n";
		std::cout << "\tbintoheader <ARGS>\n\n";
		std::cout << "The following arguments are recognized:\n\n";
		std::cout << "\t-i\tThe input file\n";
		std::cout << "\t-o\tThe output file [defaulted to '<input>.h']\n";
		std::cout << "\t-name\tThe name of the c array\n";
		std::cout << "\t\tDefaulted to '<input>.h' with path stripped all '.' replaced by '.'\n";
		std::cout << "\t-size\tThe size of the array values in bits. Defaulted to 8.\n";
		std::cout << "\t\tThe file size must be a multiple of the size value\n";
		std::cout << "\t-help\tShow this error message\n\n";
		std::cout << "The program is released into the public domain\n";
		std::cout << "A copy of the source can be obtained under\n";
		std::cout << "https://github.com/nyorain/bintoheader\n";
		return 0;
	}

	// input, output
	auto input = args.get<std::string>("i", {});
	auto output = args.get<std::string>("o", {});
	const auto& pos = args.positional();

	auto current = 0u;
	if(input.empty() && pos.size() > current)
		input = pos[current++];

	if(output.empty() && pos.size() > current)
		output = pos[current++];

	if(output.empty())
		output = input + ".h";

	// name, size
	auto name = args.get<std::string>("name", {});
	if(name.empty()) {
		name = input + "_data";
		stripPath(name);
		replace(name, ".", "_");
	}

	if(name.empty()) {
		std::cout << "Empty array name is invalid!\n";
		return 1;
	}

	auto size = args.get<unsigned int>("size", 8);
	if(size != 8 && size != 16 && size != 32 && size != 64) {
		std::cout << "Invalid size parameter\n";
		std::cout << "Must be one of {8, 16, 32, 64}\n";
		return 1;
	}

	auto data = loadFile(input);
	outputData(data, output, name, size);
	std::cout << "Succesfully created " << name << ": " << data.size () << " bytes\n";
}
