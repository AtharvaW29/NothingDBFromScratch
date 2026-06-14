#pragma once

#include <vector>

namespace NothingDB {

	template<typename KeyType, typename ValueType>
	class BPlusTreeNode {
	public:
		virtual ~BPlusTreeNode() = default;
		bool is_leaf_;
		std::vector<KeyType> keys_;
	};

	template<typename KeyType, typename ValueType>
	class BPlusTreeLeafNode : public BPlusTreeNode<KeyType, ValueType>
	{
	public:
		std::vector<ValueType> values_;
		BPlusTreeLeafNode* next_;
		BPlusTreeLeafNode()
		{
			this->is_leaf_ = true;
			this->next_ = nullptr;
		}
	};

	template<typename KeyType, typename ValueType>
	class BPlusTreeInternalNode : public BPlusTreeNode<KeyType, ValueType>
	{
	public:
		std::vector<BPlusTreeNode<KeyType, ValueType>*>children_;
		BPlusTreeInternalNode()
		{
			this->is_leaf_ = false;
		}
	};
}
