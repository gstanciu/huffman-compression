#include "Node.h"

namespace Huffman
{
	Node::Node(int character, uint64_t frequency,
		std::shared_ptr<Node> left,
		std::shared_ptr<Node> right) :
		m_character(character),
		m_frequency(frequency),
		m_left(left),
		m_right(right) {}

	uint64_t Node::getFrequency() const { return m_frequency; }
	bool Node::isLeaf() const { return !(m_left && m_right); }
}