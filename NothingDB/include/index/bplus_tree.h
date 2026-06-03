#include "index/bplus_tree_node.h"

namespace NothinDB{
	template<typename KeyType, typename ValueType>
	class BPlusTree {
	public:
		BPlusTree();

		void Insert(const KeyType& key, const ValueType& value);
		bool Search(const KeyType& key, ValueType& value);
	private:
		BPlusTreeNode<KeyType, ValueType>* root_;
	};
}