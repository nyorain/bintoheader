#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cstring>

//beginning of the file to output
constexpr auto fileTemplate = 1 + R"SRC(
#ifndef BINARY_DATA_HEADER_INCLUDE
#define BINARY_DATA_HEADER_INCLUDE

#include <stdint.h>

//just check for c++11
#if defined(__cplusplus) && __cplusplus >= 201103L
	constexpr	
#else
	const
#endif

%t binaryData[] = {
	%d
};

#endif //header guard)SRC";

void replace(std::string& str, const std::string& from, const std::string& to) 
{
    if(from.empty()) return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) 
	{
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

//how many chars should be in one line (excluding beginning tab).
constexpr auto lineLength = 80;

std::string parseFileName(const std::string& name)
{
	auto pos1 = name.find_last_of('\\');
	auto pos2 = name.find_last_of('/');

	if(pos1 == std::string::npos) pos1 = 0;
	else ++pos1;
	if(pos2 == std::string::npos) pos2 = 0;
	else ++pos2;

	auto ret = name.substr(std::max(pos1, pos2)); 
	std::cout << "outputting to " << ret << "\n";
	return ret;
}

std::vector<std::uint8_t> loadFile(const char* name)
{
	const static std::string errorMsg1 = " loadFile: couldnt open file ";
	const static std::string errorMsg2 = " loadFile: failed to read file ";

	auto openmode = std::ios::ate | std::ios::binary;

	std::ifstream ifs(name, openmode);
	if(!ifs.is_open()) throw std::runtime_error(std::strerror(errno) + errorMsg1 + name);

	auto size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::vector<std::uint8_t> buffer(size);
	auto data = reinterpret_cast<char*>(buffer.data());
	if(!ifs.read(data, size)) throw std::runtime_error(std::strerror(errno) + errorMsg2 + name);

	return buffer;
}

void outputData(const std::vector<std::uint8_t>& data, const char* name, int size)
{
	auto fname = parseFileName(std::string(name) + ".h");
	std::ofstream ofs(fname);
	if(!ofs.is_open()) throw std::runtime_error("Couldnt open output file " + fname);

	auto byteSize = size / 8;
	if(data.size() % byteSize) 
		throw std::runtime_error("File size doesnt match outputType");

	std::string dataString;
	dataString.reserve(data.size() * 5); //approx.

	auto currColSize = 0u;
	for(auto i = 0u; i < data.size() / byteSize; ++i)
	{
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

	std::string txt = fileTemplate;
	replace(txt, "%t", outputTypeName);
	replace(txt, "%d", dataString);

	ofs << txt;
}

void printUsage()
{
	std::cerr << "Usage: bintoheader <file> [<file> ...] [-s{8|16|32|64} = 8]\n";
	std::cerr << "-s\tSpecifies the size of the output data array in bits (Default = 8).\n";
	std::cerr << "\tWill fail if the file size is not a multiple of the specified size\n";
}


int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printUsage();
		return EXIT_FAILURE;
	}

	std::uint8_t size = 8;
	int nextSize = 0;
	int nextSizeArg = 0;
	int sizeArg = 0;
	for(auto i = 1; i < argc; ++i)
	{
		if(std::strncmp(argv[i], "-s", 2) == 0)
		{
			if(nextSize > 0)
			{
				std::cerr << "Two size parameters given\n";
				printUsage();
				return EXIT_FAILURE;
			}

			if(std::strlen(argv[i]) > 2)
			{
				try
				{
					size = std::stoi(argv[i] + 2);
				}
				catch(...)
				{
					std::cerr << "Invalid size parameter given.\n";
					printUsage();
					return EXIT_FAILURE;
				}
				nextSize = 2;
				nextSizeArg = i;
				sizeArg = i;
				continue;
			}
			else
			{
				nextSizeArg = i;
				nextSize = 1;
				continue;
			}
		}

		if(nextSize == 1)
		{
			try
			{
				size = std::stoi(argv[i]);
			}
			catch(...)
			{
				std::cerr << "Invalid size parameter given.\n";
				printUsage();
				return EXIT_FAILURE;
			}

			nextSize = 2;
			sizeArg = i;
			continue;
		}
	}

	if(!(size == 8 || size == 16 || size == 32 || size == 64))
	{
		std::cerr << "Invalid size paramter.\n";
		printUsage();
		return EXIT_FAILURE;
	}

	auto fileCount = 0u;
	for(auto i = 1; i < argc; ++i)
	{
		if(i == sizeArg || i == nextSizeArg) continue;
		auto file = argv[i];
		auto data = loadFile(file);
		outputData(data, file, size);

		std::cout << "Succesfully parsed " << file << ". " << data.size () << " bytes\n";
		++fileCount;
	}

	if(!fileCount)
	{
		std::cerr << "Error: no input files given\n";
		printUsage();
		return EXIT_FAILURE;
	}
}
