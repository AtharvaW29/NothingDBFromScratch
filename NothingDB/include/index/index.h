#pragma once
#include "index/bplus_tree.h"
#include "storage/rid.h"

namespace NothingDB{
	template<typename KeyType>
	class Index
	{
	public:
		void Insert(const KeyType& key, const RID& rid)
		{
			tree_.Insert(key, rid);
		}
		bool Search(
			const KeyType& key, RID& rid)
		{
			return tree_.Search(key, rid);
		}
	private:
		BPlusTree<KeyType, RID> tree_;
	};
}