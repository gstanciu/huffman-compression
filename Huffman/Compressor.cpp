#include <climits>
#include <fstream>
#include "Compressor.h"

namespace Huffman
{
	Compressor::Compressor(const std::string& inputFilename) :
		m_inputFilename(inputFilename) 
	{
		std::ifstream file(m_inputFilename, std::ifstream::binary);
		std::vector<char> buffer(4096, 0);

		while (file.good() && !file.eof())
		{
			file.read(buffer.data(), buffer.size());
			auto size = file.gcount();
			for (int i = 0; i < size; i++)
			{
				const auto it = m_frequencies.find(buffer[i]);
				if (it != m_frequencies.cend())
					it->second++;
				else
					m_frequencies[buffer[i]] = 1;
			}
		}
		m_frequencies[Tree::LAST] = 1;

		m_tree.buildFromFrequencyMap(m_frequencies);
	}

	void Compressor::compressTo(const std::string& outputFilename)
	{
		std::ofstream out(outputFilename, std::ifstream::binary);
		out << m_tree.serialize() << "\n";

		std::ifstream in(m_inputFilename, std::ifstream::binary);
		std::vector<char> buffer(4096, 0);

		char byteToWrite = 0;
		int writtenBits = 0;
		bool writtenLast = false;

		while (writtenLast == false)
		{
			std::streamsize size;
			if (in.good() && !in.eof())
			{
				in.read(buffer.data(), buffer.size());
				size = in.gcount();
			}
			else
			{
				writtenLast = true;
				size = 1;
			}
			for (int i = 0; i < size; i++)
			{
				const auto& encodingMap = m_tree.getEncodingMap();
				Tree::EncodingMap::const_iterator encoding;
				if (writtenLast)
				{
					encoding = encodingMap.find(Tree::LAST);
				}
				else
				{
					encoding = encodingMap.find(buffer[i]);
				}
				for (const auto& ch : encoding->second)
				{
					int bit = (ch == '1');
					byteToWrite |= (bit << writtenBits);
					writtenBits++;
					if ((writtenBits == CHAR_BIT))
					{
						out << byteToWrite;
						byteToWrite = 0;
						writtenBits = 0;
					}
				}
			}
		}
		out << byteToWrite;
	}
}
