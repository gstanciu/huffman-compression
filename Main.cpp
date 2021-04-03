#include <iostream>
#include "Huffman/Compressor.h"
#include "Huffman/Decompressor.h"

void printUsage()
{
	std::cout << "Usage:\ncompress: -c in out \ndecompress: -d in out" << "\n";
}

int main(int argc, char* argv[])
{
	bool compress = true;
	std::string inputFile;
	std::string outputFile; 

	if (argc == 4)
	{
		std::string first = argv[1];
		if (first == "-c")
			compress = true;
		else
			if (first == "-d")
				compress = false;
			else
			{
				std::cerr << "Invalid first parameter\n";
				printUsage();
				return 0;
			}
		inputFile = argv[2];
		outputFile = argv[3];
	}
	else
	{
		printUsage();
		return 0;
	}

	if (compress)
	try
	{
		Huffman::Compressor compressor(inputFile);
		compressor.compressTo(outputFile);
		std::cout << "Compression succesful!" << "\n";
	}
	catch (const std::exception & e)
	{
		std::cerr << "Compression failed: " << e.what() << "\n";
	}
	else
	try
	{
		Huffman::Decompressor decompressor(inputFile);
		decompressor.decompressTo(outputFile);
		std::cout << "Decompression succesful!" << "\n";
	}
	catch (const std::exception & e)
	{
		std::cerr << "Decompression failed: " << e.what() << "\n";
	}
	return 0;
}
