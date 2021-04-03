#pragma once
#include <memory>

namespace Huffman
{
	struct Node
	{
		Node(int character, uint64_t frequency,
			 std::shared_ptr<Node> left = nullptr,
			 std::shared_ptr<Node> right = nullptr);
		Node(const Node&) = delete;
		Node& operator= (const Node&) = delete;

		uint64_t getFrequency() const;
		bool isLeaf() const;

		uint64_t m_frequency;
		int m_character;
		std::shared_ptr<Node> m_left, m_right;
	};
}