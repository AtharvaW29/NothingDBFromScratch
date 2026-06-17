// NothingDB.cpp : Defines the entry point
#include<array>;

#include "NothingDB.h"
#include "storage/page.h"
#include "storage/disk_manager.h"
#include "buffer/lru_replacer.h"
#include "buffer/bufferpool_manager.h"
#include "storage/table_page.h"
#include "catalog/schema.h"
#include "storage/tuple.h"
#include "storage/table_heap.h"
#include "index/bplus_tree.h"
#include <storage/rid.h>


using namespace NothingDB;

int main()
{

	BPlusTree<int, RID> bptree;
	for (int i = 0; i <= 100; i++)
	{
		bptree.Insert(i, RID(i, i));
	}

	bptree.PrintTree();
	for(int i = 0; i <= 100; i++)
	{
		RID rid;
		bool found = bptree.Search(i, rid);
		if(!found)
		{
			std::cout << "Missing" << i << std::endl;
		}
	}
	std::cout << "Test Sequential Traversal ->" << std::endl;
	for(auto iter = bptree.Begin(); iter != bptree.End(); ++iter)
	{
		auto [key, rid] = *iter;
		std::cout << key << " ";
	}
    return 0;
}
