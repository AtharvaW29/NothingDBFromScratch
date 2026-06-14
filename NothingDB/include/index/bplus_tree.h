#pragma once

#include <vector>
#include "index/bplus_tree_node.h"

namespace NothingDB {

	template<typename KeyType, typename ValueType>
	class BPlusTree
	{
		public:
			BPlusTree();

			void Insert(
				const KeyType& key,
				const ValueType& value);

			bool Search(
				const KeyType& key,
				ValueType& value);

			static constexpr int ORDER = 4;
			void SplitRootLeaf();

			// for debugging
			void PrintTree();
	private:
		BPlusTreeNode<KeyType, ValueType>* root_;
	};

}

#include "index/bplus_tree.tpp"