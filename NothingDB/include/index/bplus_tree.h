#pragma once

#include <vector>
#include<iostream>
#include "index/bplus_tree_node.h"
#include "index/bplus_tree_iterator.h"

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

			void SplitLeaf(BPlusTreeLeafNode<KeyType, ValueType>* leaf);

			void SplitInternal(BPlusTreeInternalNode<KeyType, ValueType>* node);

			BPlusTreeIterator<KeyType, ValueType> Begin();

			BPlusTreeIterator<KeyType, ValueType> End();

			BPlusTreeLeafNode<KeyType, ValueType>* GetLeftmostLeaf();

			std::vector<ValueType>RangeScan(const KeyType& start, const KeyType& end);

			// for debugging
			void PrintTree();
	private:
		BPlusTreeNode<KeyType, ValueType>* root_;
		BPlusTreeLeafNode<KeyType, ValueType>* FindLeaf(const KeyType& key);
	};

}

#include "index/bplus_tree.tpp"


/* Split flow logic for now
Insert
   |
  \|/
Leaf Overflow
   |
  \|/
SplitLeaf
   |
  \|/
Parent Overflow?
   |
  \|/
SplitInternal
   |
  \|/
Grandparent Overflow?
   |
  \|/
SplitInternal
   |
  \|/
...
SplitInternal will recursively handle splits as the tree grows
*/