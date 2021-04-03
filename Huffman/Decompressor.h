#pragma once
#include <fstream>
#include "Tree.h"

namespace Huffman
{
	class Decompressor
	{
	public:
		Decompressor(const std::string& inputFilename);
		void decompressTo(const std::string& outputFilename);

	private:
		Tree m_tree;
		std::string m_inputFilename;
		std::ifstream m_file;
	};
}