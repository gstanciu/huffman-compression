#pragma once
#include "Tree.h"

namespace Huffman
{
	class Compressor
	{
	public:
		Compressor(const std::string& inputFilename);
		void compressTo(const std::string& outputFilename);

	private:
		Tree::FrequencyMap m_frequencies;
		Tree m_tree;
		std::string m_inputFilename;
	};
}