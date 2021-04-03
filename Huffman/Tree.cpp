#include <queue>
#include "Tree.h"

constexpr int INTERNAL = -999;

namespace Huffman
{
	const int Tree::LAST = 999;

	Tree::Tree()
	{
	}

	void Tree::buildFromFrequencyMap(const FrequencyMap& frequencies)
	{
		using NodePtr = std::shared_ptr<Huffman::Node>;

		auto compareNodePtr = [](const NodePtr& l, const NodePtr& r)
		{
			return l->getFrequency() > r->getFrequency();
		};

		std::priority_queue<NodePtr, std::vector<NodePtr>, decltype(compareNodePtr)> minHeap(compareNodePtr);

		for (const auto& elem : frequencies)
			minHeap.push(std::make_shared<Node>(elem.first, elem.second));

		while (minHeap.size() > 1)
		{
			auto first = minHeap.top();
			minHeap.pop();
			auto second = minHeap.top();
			minHeap.pop();

			minHeap.emplace(std::make_shared<Node>(
							'-',
							first->getFrequency() + second->getFrequency(),
							second,
							first));
		}

		m_root = minHeap.top();
		traverse(m_root);
	}

	const Tree::EncodingMap& Tree::getEncodingMap() const { return m_encodingMap; }
	std::shared_ptr<const Node> Tree::getRoot() const { return m_root; }

	std::string Tree::serialize(std::shared_ptr<const Node> root) const
	{
		if (root->isLeaf())
		{
			return std::to_string(root->m_character);
		}
		else
		{
			return "#," + serialize(root->m_left) + "," + serialize(root->m_right);
		}
	}

	std::string Tree::serialize() const
	{
		return serialize(m_root);
	}

	Tree Tree::deserialize(const std::string& data)
	{
		std::shared_ptr<Node> root = std::make_shared<Node>(0, 0);
		std::vector<int> leafCharacters;

		for (std::size_t i = 0; i < data.size(); ++i)
		{
			if (data[i] == ',')
				continue;
			if (data[i] == '#')
			{
				leafCharacters.push_back(INTERNAL);
			}
			else
			{
				bool negative = false;
				if (data[i] == '-')
				{
					negative = true;
					i++;
				}
				std::string nr;
				while (i < data.size() && data[i] >= '0' && data[i] <= '9')
				{
					nr += data[i];
					i++;
				}
				auto intNr = std::stoi(nr);
				if (negative)
					intNr = -intNr;
				leafCharacters.push_back(intNr);
			}
		}

		Tree tree;
		std::size_t idx = 0;
		if (!leafCharacters.empty())
			tree.m_root = buildNode(leafCharacters, idx);
		return tree;
	}

	void Tree::traverse(std::shared_ptr<const Node> root, std::string encoding)
	{
		if (root)
		{
			if (root->isLeaf())
			{
				m_encodingMap[root->m_character] = encoding;
			}
			else
			{
				traverse(root->m_left, encoding + "0");
				traverse(root->m_right, encoding + "1");
			}
		}
	}

	std::shared_ptr<Node> Tree::buildNode(const std::vector<int> leafCharacters, std::size_t& idx)
	{
		std::shared_ptr<Node> node = std::make_shared<Node>(leafCharacters[idx], 0);
		idx++;
		if (node->m_character == INTERNAL)
		{
			node->m_left = buildNode(leafCharacters, idx);
			node->m_right = buildNode(leafCharacters, idx);
		}
		return node;
	}
}
