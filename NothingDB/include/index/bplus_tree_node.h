#pragma once
#include <vector>

namespace NothingDB {

	template<typename KeyType, typename ValueType>
	class BPlusTreeNode {
	public:
		bool is_leaf;
		
		std::vector<KeyType> keys_;
		
		virtual ~BPlusTreeNode() = default;
	};

	template<typename KeyType, typename ValueType>
	class BPlusTreeLeafNode : public BPlusTreeNode<KeyType, ValueType>
	{
	public:
		std::vector<ValueType> values_;
		BPlusTreeLeafNode* next_;
		BPlusTreeLeafNode()
		{
			this->is_leaf = true;
			next_ = nullptr;
		}
	};

	template<typename KeyType, typename ValueType>
	class BPlusTreeInternalNode : public BPlusTreeNode<KeyType, ValueType>
	{
	public:
		std::vector<BPlusTreeNode<KeyType, ValueType>*> children_;
		BPlusTreeInternalNode()
		{
			this->is_leaf = false;
		}
	};
}
