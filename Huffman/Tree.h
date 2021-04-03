#pragma once
#include <string>
#include <vector>
#include <map>
#include "Node.h"

namespace Huffman
{
	class Tree
	{
	public:
		static const int LAST;

		using EncodingMap = std::map<int, std::string>;
		using FrequencyMap = std::map<int, uint64_t>;

		Tree();
		Tree(const Tree&) = delete;
		Tree(Tree&&) = default;
		Tree& operator=(const Tree&) = delete;
		Tree& operator=(Tree&&) = default;

		void buildFromFrequencyMap(const FrequencyMap& frequencies);
		const EncodingMap& getEncodingMap() const;
		std::shared_ptr<const Node> getRoot() const;
		std::string serialize() const;
		static Tree deserialize(const std::string& data);

	private:
		std::string serialize(std::shared_ptr<const Node>) const;
		static std::shared_ptr<Node> buildNode(const std::vector<int> leafCharacters, std::size_t& idx);
		void traverse(std::shared_ptr<const Node>, std::string encoding = "");
		std::shared_ptr<const Node> m_root;
		EncodingMap m_encodingMap;
	};
}