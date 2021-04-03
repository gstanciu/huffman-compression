#include <fstream>
#include <sstream>
#include "Decompressor.h"

namespace Huffman
{
	Decompressor::Decompressor(const std::string& inputFilename) :
		m_inputFilename(inputFilename)
	{
		m_file = std::ifstream(m_inputFilename, std::ifstream::binary);

		std::string header;
		std::getline(m_file, header);
		
		m_tree = Tree::deserialize(header);
	}

	void Decompressor::decompressTo(const std::string& outputFilename)
	{
		std::ofstream out(outputFilename, std::ifstream::binary);
		std::vector<char> buffer(4096, 0);

		char currentByte;
		int remainingBits = 0; 

		int idx = 0;

		auto root = m_tree.getRoot();

		while (1)
		{
			if (!out.good())
				break;
			auto readNextByte = [&]() {
				if (idx == 0 || idx == buffer.size())
				{
					idx = 0;
					m_file.read(buffer.data(), buffer.size());
					std::size_t read = static_cast<std::size_t>(m_file.gcount());
					if (read != buffer.size())
						buffer.resize(read);
				}
				
				if (buffer.size() == 0)
					throw std::runtime_error("Corrupted file!");
				auto ret = buffer[idx];
				idx++;
				return ret;
			};

			if (remainingBits == 0)
			{
				remainingBits = 8;
				bool error = false;
				currentByte = readNextByte();
			}
			--remainingBits;
			int bit =  (currentByte >> (7 - remainingBits)) & 1;

			if (root->isLeaf())
			{
				if (root->m_character != Tree::LAST)
					out << static_cast<char>(root->m_character);
				else
				{
					if (m_file.eof())
						return;
				}
				root = m_tree.getRoot();
			}
			if (bit == 0)
				root = root->m_left;
			else
				root = root->m_right;
		}
	}
}